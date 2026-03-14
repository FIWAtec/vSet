#include "widgets/DeckWidget.h"

#include <QFileDialog>
#include <QFileInfo>
#include <QGridLayout>
#include <QGroupBox>
#include <QLabel>
#include <QPushButton>
#include <QSignalBlocker>
#include <QSlider>
#include <QVBoxLayout>

namespace
{
QString formatMs(qint64 ms)
{
    const auto totalSeconds = ms / 1000;
    const auto minutes = totalSeconds / 60;
    const auto seconds = totalSeconds % 60;
    return QString("%1:%2").arg(minutes, 2, 10, QChar('0')).arg(seconds, 2, 10, QChar('0'));
}
}

DeckWidget::DeckWidget(DeckController* controller, QWidget* parent)
    : QWidget(parent)
    , m_controller(controller)
{
    auto* root = new QVBoxLayout(this);

    auto* title = new QLabel(controller->deckName());
    title->setObjectName("SectionTitle");

    m_trackLabel = new QLabel("Kein Track geladen");
    m_timeLabel = new QLabel("00:00 / 00:00");

    auto* loadButton = new QPushButton("Datei laden");
    auto* playButton = new QPushButton("Play");
    auto* pauseButton = new QPushButton("Pause");
    auto* stopButton = new QPushButton("Stop");

    auto* loopInButton = new QPushButton("Loop In");
    auto* loopOutButton = new QPushButton("Loop Out");
    auto* loopClearButton = new QPushButton("Loop Clear");

    m_positionSlider = new QSlider(Qt::Horizontal);
    m_positionSlider->setRange(0, 0);

    m_volumeSlider = new QSlider(Qt::Horizontal);
    m_volumeSlider->setRange(0, 100);
    m_volumeSlider->setValue(controller->volumePercent());

    auto* grid = new QGridLayout();
    grid->addWidget(loadButton, 0, 0, 1, 2);
    grid->addWidget(playButton, 1, 0);
    grid->addWidget(pauseButton, 1, 1);
    grid->addWidget(stopButton, 1, 2);
    grid->addWidget(loopInButton, 2, 0);
    grid->addWidget(loopOutButton, 2, 1);
    grid->addWidget(loopClearButton, 2, 2);

    for (int i = 0; i < 4; ++i)
    {
        auto* saveCue = new QPushButton("Save Cue " + QString::number(i + 1));
        auto* jumpCue = new QPushButton("Go Cue " + QString::number(i + 1));

        grid->addWidget(saveCue, 3 + i, 0, 1, 2);
        grid->addWidget(jumpCue, 3 + i, 2, 1, 1);

        connect(saveCue, &QPushButton::clicked, this, [this, i]()
        {
            m_controller->saveCue(i);
        });

        connect(jumpCue, &QPushButton::clicked, this, [this, i]()
        {
            m_controller->jumpToCue(i);
        });
    }

    root->addWidget(title);
    root->addWidget(m_trackLabel);
    root->addWidget(m_timeLabel);
    root->addWidget(m_positionSlider);
    root->addLayout(grid);
    root->addWidget(new QLabel("Deck Volume"));
    root->addWidget(m_volumeSlider);

    connect(loadButton, &QPushButton::clicked, this, [this]()
    {
        const auto path = QFileDialog::getOpenFileName(
            this,
            "Audio-Datei laden",
            QString(),
            "Audio (*.mp3 *.wav *.flac *.ogg *.m4a)"
        );

        if (!path.isEmpty())
        {
            m_controller->loadFile(path);
        }
    });

    connect(playButton, &QPushButton::clicked, m_controller, &DeckController::play);
    connect(pauseButton, &QPushButton::clicked, m_controller, &DeckController::pause);
    connect(stopButton, &QPushButton::clicked, m_controller, &DeckController::stop);

    connect(loopInButton, &QPushButton::clicked, m_controller, &DeckController::setLoopIn);
    connect(loopOutButton, &QPushButton::clicked, m_controller, &DeckController::setLoopOut);
    connect(loopClearButton, &QPushButton::clicked, m_controller, &DeckController::clearLoop);

    connect(m_volumeSlider, &QSlider::valueChanged, m_controller, &DeckController::setVolume);

    connect(m_positionSlider, &QSlider::sliderMoved, this, [this](int value)
    {
        m_controller->setPosition(static_cast<qint64>(value));
    });

    connect(m_controller, &DeckController::trackLoaded, this, [this](const QString& path)
    {
        m_trackLabel->setText(QFileInfo(path).fileName());
    });

    connect(m_controller, &DeckController::positionChanged, this, [this](qint64 position)
    {
        QSignalBlocker blocker(m_positionSlider);
        m_positionSlider->setValue(static_cast<int>(position));
        updateTimeLabel(position, m_controller->duration());
    });

    connect(m_controller, &DeckController::durationChanged, this, [this](qint64 duration)
    {
        QSignalBlocker blocker(m_positionSlider);
        m_positionSlider->setRange(0, static_cast<int>(duration));
        updateTimeLabel(m_controller->position(), duration);
    });
}

void DeckWidget::updateTimeLabel(qint64 position, qint64 duration)
{
    m_timeLabel->setText(formatMs(position) + " / " + formatMs(duration));
}
