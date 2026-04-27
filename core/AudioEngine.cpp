// AudioEngine.cpp
// TSF_IMPLEMENTATION must be defined in exactly one translation unit.
#define TSF_IMPLEMENTATION
#include "tsf.h"

#include "AudioEngine.h"
#include <QAudioFormat>
#include <QAudioSink>
#include <QBuffer>
#include <QCoreApplication>
#include <QDebug>
#include <QTimer>

static constexpr int SAMPLE_RATE = 44100;
static constexpr int CHANNELS    = 2;

// Semitone offsets from C for each diatonic degree: C D E F G A B
static constexpr int diatonicSemitones[] = {0, 2, 4, 5, 7, 9, 11};

AudioEngine::AudioEngine(QObject* parent) : QObject(parent)
{
    // Try several runtime locations for piano.sf2
    const QString appDir = QCoreApplication::applicationDirPath();
    const QStringList candidates = {
        appDir + "/piano.sf2",
        appDir + "/soundfonts/piano.sf2",
        "piano.sf2",
        "soundfonts/piano.sf2",
    };

    for (const QString& path : candidates) {
        m_tsf = tsf_load_filename(path.toLocal8Bit().constData());
        if (m_tsf) {
            tsf_set_output(m_tsf, TSF_STEREO_INTERLEAVED, SAMPLE_RATE, 0.0f);
            qDebug() << "AudioEngine: loaded SoundFont from" << path;
            return;
        }
    }

    qWarning() << "AudioEngine: piano.sf2 not found — playback disabled.";
    qWarning() << "  Place piano.sf2 next to the executable or in a soundfonts/ subdirectory.";
    qWarning() << "  Free option: download GeneralUser GS or FluidR3_GM.sf2 and rename it piano.sf2.";
}

AudioEngine::~AudioEngine()
{
    if (m_tsf) tsf_close(m_tsf);
}

bool AudioEngine::isReady() const
{
    return m_tsf != nullptr;
}

int AudioEngine::rowToMidi(int row, int acc)
{
    const int octave = row / 7;
    const int degree = row % 7;
    return 60 + octave * 12 + diatonicSemitones[degree] + acc; // 60 = C4
}

void AudioEngine::playNotes(const QList<QVariantMap>& notes, float durationSeconds)
{
    if (!m_tsf || notes.isEmpty()) return;

    const int totalSamples = static_cast<int>(SAMPLE_RATE * durationSeconds);

    for (const QVariantMap& note : notes)
        tsf_note_on(m_tsf, 0, rowToMidi(note["row"].toInt(), note["acc"].toInt()), 1.0f);

    QByteArray pcm(totalSamples * CHANNELS * static_cast<int>(sizeof(short)), '\0');
    tsf_render_short(m_tsf, reinterpret_cast<short*>(pcm.data()), totalSamples, 0);

    for (const QVariantMap& note : notes)
        tsf_note_off(m_tsf, 0, rowToMidi(note["row"].toInt(), note["acc"].toInt()));

    playPcm(pcm);
}

// ── Private: send pre-rendered PCM to QAudioSink asynchronously ─────────────

void AudioEngine::playPcm(const QByteArray& pcm)
{
    // Stop any existing playback before starting new audio.
    // m_currentSink->stop() emits stateChanged(StoppedState) synchronously,
    // which causes the lambda below to clean up and null the member pointers.
    if (m_currentSink)
        m_currentSink->stop();

    QAudioFormat format;
    format.setSampleRate(SAMPLE_RATE);
    format.setChannelCount(CHANNELS);
    format.setSampleFormat(QAudioFormat::Int16);

    m_currentBuf = new QBuffer(this);
    m_currentBuf->setData(pcm);
    m_currentBuf->open(QIODevice::ReadOnly);

    m_currentSink = new QAudioSink(format, this);

    // Capture local copies so the lambda identifies which sink/buf it belongs to
    QAudioSink* captureSink = m_currentSink;
    QBuffer*    captureBuf  = m_currentBuf;

    connect(m_currentSink, &QAudioSink::stateChanged, this,
            [this, captureSink, captureBuf](QAudio::State state) {
                // Handle both natural end (IdleState) and explicit stop (StoppedState)
                if (state == QAudio::IdleState || state == QAudio::StoppedState) {
                    if (m_currentSink == captureSink) {
                        m_currentSink = nullptr;
                        m_currentBuf  = nullptr;
                    }
                    captureBuf->close();
                    captureSink->deleteLater();
                    captureBuf->deleteLater();
                }
            });

    m_currentSink->start(m_currentBuf);

    if (m_currentSink && m_currentSink->error() != QAudio::NoError)
        qWarning() << "AudioEngine: playback error —" << m_currentSink->error()
                   << "(is /dev/snd accessible inside Docker?)";
}

void AudioEngine::stop()
{
    if (!m_currentSink) return;
    ++m_playbackGeneration;       // invalidate any pending beat-cursor timers
    m_currentSink->stop();        // emits stateChanged(StoppedState) → lambda cleans up
}

// ── Public: duration helper ──────────────────────────────────────────────────

double AudioEngine::lengthToSeconds(int length, double bpm)
{
    const double quarter = 60.0 / bpm;
    switch (length) {
        case 1: return quarter / 2.0; // eighth
        case 2: return quarter;        // quarter
        case 3: return quarter * 2.0;  // half
        case 4: return quarter * 4.0;  // whole
        default:
            qWarning() << "AudioEngine: unknown length" << length << "— defaulting to quarter note";
            return quarter;
    }
}

// ── Public: continuous-buffer sequence playback ───────────────────────────────
// Renders the entire sequence into ONE PCM buffer and plays it with a single
// QAudioSink call, eliminating per-beat timer jitter.

void AudioEngine::playSequence(const QList<QVariantMap>& notes, double bpm)
{
    if (!m_tsf || notes.isEmpty()) return;

    // ── Step 1: group notes by beat (input is already in beat order) ─────────
    struct BeatGroup {
        int beat    = 0;
        int lenId   = 2;   // UI type-ID: 1=eighth,2=quarter,3=half,4=whole
        QList<QVariantMap> notes;
    };
    QList<BeatGroup> groups;
    for (const QVariantMap& note : notes) {
        const int beat = note["beat"].toInt();
        if (groups.isEmpty() || groups.last().beat != beat)
            groups.append({beat, note["length"].toInt(), {}});
        groups.last().notes.append(note);
    }

    // ── Step 2: calculate total buffer length ─────────────────────────────────
    // One grid column = one eighth-note subdivision.
    // sample offset = beat * (quarter / 2) * SAMPLE_RATE
    const double eighthSec    = (60.0 / bpm) / 2.0;
    const int    eighthFrames = static_cast<int>(eighthSec * SAMPLE_RATE);
    const int    tailFrames   = static_cast<int>(0.5 * SAMPLE_RATE); // release tail

    const BeatGroup& last   = groups.last();
    const int lastOffset    = last.beat * eighthFrames;
    const int lastDurFrames = static_cast<int>(lengthToSeconds(last.lenId, bpm) * SAMPLE_RATE);
    const int totalFrames   = lastOffset + lastDurFrames + tailFrames;

    // ── Step 3: allocate one silence-filled buffer ────────────────────────────
    QByteArray pcm(totalFrames * CHANNELS * static_cast<int>(sizeof(short)), '\0');
    short* out = reinterpret_cast<short*>(pcm.data());

    // ── Step 4: walk groups in order, rendering into the buffer ──────────────
    // TSF is stateful: tsf_render_short advances its internal clock by N frames
    // regardless of the output pointer. Rendering silence gaps captures release
    // tails from previous notes and keeps TSF's timeline aligned with the buffer.
    int cursor = 0;
    for (const BeatGroup& g : groups) {
        const int targetFrame = g.beat * eighthFrames;

        // Render silence/release-tail up to this beat's start
        if (targetFrame > cursor) {
            tsf_render_short(m_tsf, out + cursor * CHANNELS, targetFrame - cursor, 0);
            cursor = targetFrame;
        }

        // Trigger all notes at this beat
        for (const QVariantMap& n : g.notes)
            tsf_note_on(m_tsf, 0, rowToMidi(n["row"].toInt(), n["acc"].toInt()), 1.0f);

        // Render the note duration directly into the buffer
        const int durFrames = static_cast<int>(lengthToSeconds(g.lenId, bpm) * SAMPLE_RATE);
        tsf_render_short(m_tsf, out + cursor * CHANNELS, durFrames, 0);

        // Release notes (begin decay)
        for (const QVariantMap& n : g.notes)
            tsf_note_off(m_tsf, 0, rowToMidi(n["row"].toInt(), n["acc"].toInt()));

        cursor += durFrames;
    }

    // Render the final release tail after the last note
    if (cursor < totalFrames)
        tsf_render_short(m_tsf, out + cursor * CHANNELS, totalFrames - cursor, 0);

    // ── Step 5: schedule UI beat cursor notifications ────────────────────────
    // Audio plays from the continuous PCM buffer — timers only drive the visual
    // highlight. Slight timer inaccuracy is acceptable for a UI cursor.
    // A generation counter ensures stale timers from a previous play/stop are ignored.
    const int gen = ++m_playbackGeneration;

    qDebug() << "[AudioEngine] playSequence: scheduling" << groups.size()
             << "beat timers, gen=" << gen;

    for (const BeatGroup& g : groups) {
        const int delayMs = static_cast<int>(g.beat * eighthSec * 1000.0);
        const int beat    = g.beat;
        QTimer::singleShot(delayMs, this, [this, gen, beat]() {
            if (m_playbackGeneration == gen) {
                qDebug() << "[AudioEngine] beatAdvanced" << beat;
                emit beatAdvanced(beat);
            } else {
                qDebug() << "[AudioEngine] timer SKIPPED beat" << beat
                         << "(gen" << gen << "vs current" << m_playbackGeneration << ")";
            }
        });
    }

    // Notify when the full sequence (including release tail) has ended
    const int endDelayMs = static_cast<int>(static_cast<double>(totalFrames) / SAMPLE_RATE * 1000.0);
    QTimer::singleShot(endDelayMs, this, [this, gen]() {
        if (m_playbackGeneration == gen) {
            qDebug() << "[AudioEngine] beatAdvanced -1 (sequence ended)";
            emit beatAdvanced(-1);
        }
    });

    playPcm(pcm);
}

// ── Public: instant note preview ─────────────────────────────────────────────

void AudioEngine::playPreview(int row, int acc)
{
    if (!m_tsf) return;

    const int midi   = rowToMidi(row, acc);
    const int frames = static_cast<int>(0.25 * SAMPLE_RATE); // 250 ms preview burst

    tsf_note_on(m_tsf, 0, midi, 1.0f);
    QByteArray pcm(frames * CHANNELS * static_cast<int>(sizeof(short)), '\0');
    tsf_render_short(m_tsf, reinterpret_cast<short*>(pcm.data()), frames, 0);
    tsf_note_off(m_tsf, 0, midi);

    playPcm(pcm);
}
