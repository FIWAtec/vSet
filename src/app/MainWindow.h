#pragma once

#include "audio/AutoMixController.h"
#include "audio/DeckController.h"
#include "services/SettingsService.h"

#include <QMainWindow>

class DeckWidget;
class MixerWidget;
class PlaylistWidget;
class SetBuilderPanel;
class YouTubePanel;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget* parent = nullptr);

private:
    void buildUi();
    void buildMenu();
    void applyCrossfader(int value);
    void loadTrackIntoDeck(DeckController* deck, const TrackItem& track);
    void showSettingsDialog();
    void refreshServicesFromSettings();
    void log(const QString& message);

    AppSettings m_settings;

    DeckController m_leftDeck;
    DeckController m_rightDeck;
    AutoMixController m_autoMix;

    DeckWidget* m_leftDeckWidget = nullptr;
    DeckWidget* m_rightDeckWidget = nullptr;
    MixerWidget* m_mixerWidget = nullptr;
    PlaylistWidget* m_playlistWidget = nullptr;
    SetBuilderPanel* m_setBuilderPanel = nullptr;
    YouTubePanel* m_youTubePanel = nullptr;
};
