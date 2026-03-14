#include "app/MainWindow.h"

#include "app/Style.h"
#include "widgets/DeckWidget.h"
#include "widgets/MixerWidget.h"
#include "widgets/PlaylistWidget.h"
#include "widgets/SetBuilderPanel.h"
#include "widgets/SettingsDialog.h"
#include "widgets/YouTubePanel.h"

#include <QAction>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QMenuBar>
#include <QPushButton>
#include <QMessageBox>
#include <QSplitter>
#include <QStatusBar>
#include <QTabWidget>
#include <QVBoxLayout>
#include <QWidget>

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
    , m_settings(SettingsService::load())
    , m_leftDeck("Deck A", this)
    , m_rightDeck("Deck B", this)
    , m_autoMix(&m_leftDeck, &m_rightDeck, this)
{
    setWindowTitle("VirtualDjSetPro");
    resize(1920, 1080);
    setStyleSheet(Style::mainStyleSheet());

    buildUi();
    buildMenu();
    refreshServicesFromSettings();

    connect(&m_leftDeck, &DeckController::statusChanged, this, &MainWindow::log);
    connect(&m_rightDeck, &DeckController::statusChanged, this, &MainWindow::log);
    connect(&m_autoMix, &AutoMixController::statusChanged, this, &MainWindow::log);

    connect(&m_autoMix, &AutoMixController::requestLoadTrack, this, [this](DeckController* deck, const TrackItem& track)
    {
        loadTrackIntoDeck(deck, track);
        deck->play();
    });

    connect(&m_autoMix, &AutoMixController::crossfaderSuggested, this, [this](int value)
    {
        m_mixerWidget->setCrossfaderValue(value);
    });

    showFullScreen();
}

void MainWindow::buildUi()
{
    auto* central = new QWidget();
    setCentralWidget(central);

    auto* root = new QVBoxLayout(central);

    auto* title = new QLabel("VirtualDjSetPro");
    title->setObjectName("TitleLabel");
    root->addWidget(title);

    auto* content = new QSplitter(Qt::Horizontal);

    auto* leftPanel = new QWidget();
    auto* leftLayout = new QVBoxLayout(leftPanel);

    m_leftDeckWidget = new DeckWidget(&m_leftDeck);
    m_rightDeckWidget = new DeckWidget(&m_rightDeck);
    m_mixerWidget = new MixerWidget();

    auto* autoMixRow = new QHBoxLayout();
    auto* autoMixStart = new QPushButton("Auto-Mix Start");
    autoMixStart->setObjectName("PrimaryButton");
    auto* autoMixStop = new QPushButton("Auto-Mix Stop");
    auto* loadToLeft = new QPushButton("Ersten Playlist-Track -> Deck A");
    auto* loadToRight = new QPushButton("Ersten Playlist-Track -> Deck B");

    autoMixRow->addWidget(autoMixStart);
    autoMixRow->addWidget(autoMixStop);
    autoMixRow->addWidget(loadToLeft);
    autoMixRow->addWidget(loadToRight);

    leftLayout->addWidget(m_leftDeckWidget);
    leftLayout->addWidget(m_mixerWidget);
    leftLayout->addWidget(m_rightDeckWidget);
    leftLayout->addLayout(autoMixRow);

    auto* rightPanel = new QTabWidget();
    m_playlistWidget = new PlaylistWidget();
    m_setBuilderPanel = new SetBuilderPanel();
    m_youTubePanel = new YouTubePanel();

    rightPanel->addTab(m_playlistWidget, "Playlist");
    rightPanel->addTab(m_setBuilderPanel, "ChatGPT Set");
    rightPanel->addTab(m_youTubePanel, "YouTube");

    content->addWidget(leftPanel);
    content->addWidget(rightPanel);
    content->setStretchFactor(0, 3);
    content->setStretchFactor(1, 2);

    root->addWidget(content);

    connect(m_mixerWidget, &MixerWidget::crossfaderChanged, this, &MainWindow::applyCrossfader);

    connect(autoMixStart, &QPushButton::clicked, this, [this]()
    {
        m_autoMix.setPlaylist(m_playlistWidget->tracks());
        m_autoMix.setCrossfadeSeconds(m_settings.defaultAutoMixSeconds);
        m_autoMix.start();
    });

    connect(autoMixStop, &QPushButton::clicked, &m_autoMix, &AutoMixController::stop);

    connect(loadToLeft, &QPushButton::clicked, this, [this]()
    {
        const auto tracks = m_playlistWidget->tracks();
        if (!tracks.isEmpty())
        {
            loadTrackIntoDeck(&m_leftDeck, tracks.first());
        }
    });

    connect(loadToRight, &QPushButton::clicked, this, [this]()
    {
        const auto tracks = m_playlistWidget->tracks();
        if (!tracks.isEmpty())
        {
            loadTrackIntoDeck(&m_rightDeck, tracks.first());
        }
    });

    connect(m_playlistWidget, &PlaylistWidget::tracksChanged, this, [this](const QList<TrackItem>& tracks)
    {
        m_setBuilderPanel->setLocalTracks(tracks);
    });

    connect(m_setBuilderPanel, &SetBuilderPanel::setPlanImported, this, [this](const QList<TrackItem>& tracks)
    {
        m_playlistWidget->setTracks(tracks);
    });

    connect(m_setBuilderPanel, &SetBuilderPanel::statusChanged, this, &MainWindow::log);
    connect(m_youTubePanel, &YouTubePanel::statusChanged, this, &MainWindow::log);

    connect(m_youTubePanel, &YouTubePanel::addYouTubeTrackRequested, this, [this](const TrackItem& track)
    {
        m_playlistWidget->appendTrack(track);
    });

    applyCrossfader(50);
}

void MainWindow::buildMenu()
{
    auto* fileMenu = menuBar()->addMenu("Datei");
    auto* settingsAction = fileMenu->addAction("Einstellungen");
    auto* exitAction = fileMenu->addAction("Beenden");

    auto* viewMenu = menuBar()->addMenu("Ansicht");
    auto* fullscreenAction = viewMenu->addAction("Fullscreen umschalten");

    auto* helpMenu = menuBar()->addMenu("Hilfe");
    auto* aboutAction = helpMenu->addAction("Über");

    connect(settingsAction, &QAction::triggered, this, &MainWindow::showSettingsDialog);
    connect(exitAction, &QAction::triggered, this, &MainWindow::close);

    connect(fullscreenAction, &QAction::triggered, this, [this]()
    {
        isFullScreen() ? showNormal() : showFullScreen();
    });

    connect(aboutAction, &QAction::triggered, this, [this]()
    {
        QMessageBox::information(
            this,
            "Über",
            "VirtualDjSetPro\n\n"
            "C++ / Qt6 Desktop-App für DJ-Set-Planung, lokale Decks, Auto-Mix und YouTube-Preview."
        );
    });
}

void MainWindow::applyCrossfader(int value)
{
    const double right = static_cast<double>(value) / 100.0;
    const double left = 1.0 - right;

    m_leftDeck.setEffectiveCrossfadeGain(left);
    m_rightDeck.setEffectiveCrossfadeGain(right);
}

void MainWindow::loadTrackIntoDeck(DeckController* deck, const TrackItem& track)
{
    if (track.source == "local")
    {
        deck->loadFile(track.filePath);

        if (track.cueMs > 0)
        {
            deck->setPosition(track.cueMs);
        }
    }
    else
    {
        log("YouTube-Titel kann nicht in lokales Deck geladen werden. Nutze die YouTube-Preview.");
    }
}

void MainWindow::showSettingsDialog()
{
    SettingsDialog dialog(m_settings, this);

    if (dialog.exec() == QDialog::Accepted)
    {
        m_settings = dialog.settings();
        SettingsService::save(m_settings);
        refreshServicesFromSettings();
        log("Einstellungen gespeichert.");
    }
}

void MainWindow::refreshServicesFromSettings()
{
    m_setBuilderPanel->setSettings(m_settings);
    m_youTubePanel->setApiKey(m_settings.youtubeApiKey);
}

void MainWindow::log(const QString& message)
{
    statusBar()->showMessage(message, 6000);
}
