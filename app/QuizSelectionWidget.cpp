#include "QuizSelectionWidget.h"
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <QFont>

struct ModeEntry {
    QuizMode    mode;
    const char* name;
    const char* description;
};

static const ModeEntry MODES[] = {
    { QuizMode::SingleNotes,   "Single Notes",
      "Place individual notes on the staff by name and beat." },
    { QuizMode::Intervals,     "Intervals",
      "Place harmonic or melodic intervals (2nd through octave)." },
    { QuizMode::Triads,        "Triads",
      "Place root-position triads as chords or ascending arpeggios." },
    { QuizMode::SeventhChords, "Seventh Chords",
      "Place seventh chords as block chords or four-note arpeggios." },
    { QuizMode::Inversions,    "Inversions",
      "Place triads in root position, first, or second inversion." },
};

static const char* BUTTON_STYLE =
    "QPushButton {"
    "  padding: 10px 14px;"
    "  border: 1px solid #b0b0b0;"
    "  border-radius: 4px;"
    "  background-color: #f5f5f5;"
    "  text-align: left;"
    "  min-height: 40px;"
    "}"
    "QPushButton:hover {"
    "  background-color: #dce8ff;"
    "  border-color: #6699cc;"
    "}";

QuizSelectionWidget::QuizSelectionWidget(QWidget* parent)
    : QWidget(parent)
{
    auto* header = new QLabel("Select a Quiz Mode", this);
    QFont hf = header->font();
    hf.setPointSize(16);
    hf.setBold(true);
    header->setFont(hf);
    header->setAlignment(Qt::AlignCenter);

    auto* sub = new QLabel("Each quiz has 10 questions.", this);
    sub->setAlignment(Qt::AlignCenter);
    sub->setStyleSheet("color: #666;");

    auto* layout = new QVBoxLayout(this);
    layout->setContentsMargins(32, 24, 32, 24);
    layout->setSpacing(10);
    layout->addWidget(header);
    layout->addWidget(sub);
    layout->addSpacing(8);

    for (const auto& entry : MODES) {
        auto* btn = new QPushButton(
            QString("%1\n%2").arg(entry.name).arg(entry.description), this);
        btn->setStyleSheet(BUTTON_STYLE);
        btn->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
        QuizMode mode = entry.mode;
        connect(btn, &QPushButton::clicked, this, [this, mode]() {
            emit modeSelected(mode);
        });
        layout->addWidget(btn);
    }

    layout->addStretch();
}
