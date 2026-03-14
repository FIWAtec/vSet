#pragma once

#include "audio/DeckController.h"
#include "model/TrackItem.h"

#include <QObject>
#include <QTimer>
#include <QQueue>

class AutoMixController : public QObject
{
    Q_OBJECT

public:
    explicit AutoMixController(
        DeckController* leftDeck,
        DeckController* rightDeck,
        QObject* parent = nullptr
    );

    void setPlaylist(const QList<TrackItem>& playlist);
    void setCrossfadeSeconds(int seconds);
    void start();
    void stop();
    bool isRunning() const;

signals:
    void requestLoadTrack(DeckController* targetDeck, const TrackItem& track);
    void crossfaderSuggested(int value);
    void statusChanged(const QString& message);

private slots:
    void onTick();

private:
    void startFirstTrackIfNeeded();
    void startNextDeck();
    DeckController* inactiveDeck() const;
    DeckController* activeDeck() const;

    DeckController* m_leftDeck = nullptr;
    DeckController* m_rightDeck = nullptr;
    QTimer m_timer;
    QList<TrackItem> m_playlist;
    int m_currentIndex = -1;
    int m_crossfadeSeconds = 10;
    bool m_running = false;
    bool m_leftActive = true;
};
