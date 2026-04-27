#pragma once
#include <QObject>
#include <QList>
#include <QVariantMap>

// Forward-declare only — full TSF definition lives in AudioEngine.cpp
struct tsf;

class QAudioSink;
class QBuffer;

class AudioEngine : public QObject {
    Q_OBJECT
public:
    explicit AudioEngine(QObject* parent = nullptr);
    ~AudioEngine() override;

    // False if the SoundFont file could not be loaded
    bool isReady() const;

    // Convert diatonic row index + accidental to MIDI note number.
    // Row 0 = C4 = MIDI 60.  acc: -1 flat, 0 natural, +1 sharp.
    static int rowToMidi(int row, int acc);

    // Render all notes simultaneously to a PCM buffer and play via QAudioSink.
    // Each map must contain "row" (int) and "acc" (int).
    void playNotes(const QList<QVariantMap>& notes, float durationSeconds = 0.75f);

    // Render the entire note sequence into one continuous PCM buffer and play it.
    // Groups stacked notes as chords; silence fills rhythmic gaps between beats.
    // One QAudioSink call — no timer jitter.
    void playSequence(const QList<QVariantMap>& notes, double bpm);

    // Immediately preview a single pitch (short burst, non-blocking).
    // Called by GridController after a note is successfully placed.
    void playPreview(int row, int acc);

    // Stop any currently playing audio immediately.
    // Safe to call when nothing is playing.
    void stop();

    // Convert UI length type-ID to seconds at the given tempo.
    // 1=eighth, 2=quarter, 3=half, 4=whole. Unknown lengths default to quarter.
    static double lengthToSeconds(int length, double bpm);

signals:
    // Fires (via QTimer) at the start of each beat during sequence playback.
    // Fires with beat = -1 when the sequence has finished playing.
    void beatAdvanced(int beat);

private:
    // Send a pre-rendered PCM buffer to QAudioSink for async playback.
    // Stops any previously active sink first.
    void playPcm(const QByteArray& pcm);

    tsf*        m_tsf               = nullptr;
    QAudioSink* m_currentSink       = nullptr;
    QBuffer*    m_currentBuf        = nullptr;
    // Incremented on each stop() or new playSequence(); pending beat timers
    // compare against this value and skip their emit if it no longer matches.
    int         m_playbackGeneration = 0;
};
