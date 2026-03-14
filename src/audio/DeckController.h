#pragma once

#include <QObject>
#include <QAudioOutput>
#include <QMediaPlayer>
#include <QTimer>
#include <QUrl>

class DeckController : public QObject
{
    Q_OBJECT

public:
    explicit DeckController(const QString& deckName, QObject* parent = nullptr);

    void loadFile(const QString& filePath);
    void play();
    void pause();
    void stop();

    void setVolume(int percent);
    void setPosition(qint64 ms);

    void setLoopIn();
    void setLoopOut();
    void clearLoop();

    void saveCue(int slot);
    void jumpToCue(int slot);

    void setEffectiveCrossfadeGain(double gain);

    QString deckName() const;
    QString currentFilePath() const;
    qint64 position() const;
    qint64 duration() const;
    bool isPlaying() const;
    int volumePercent() const;

signals:
    void positionChanged(qint64 positionMs);
    void durationChanged(qint64 durationMs);
    void playbackStateChanged(bool isPlaying);
    void trackLoaded(const QString& path);
    void statusChanged(const QString& message);

private slots:
    void onLoopTick();

private:
    void applyVolume();

    QString m_deckName;
    QString m_currentFilePath;
    QMediaPlayer m_player;
    QAudioOutput m_output;
    QTimer m_loopTimer;

    int m_userVolumePercent = 80;
    double m_crossfadeGain = 1.0;

    qint64 m_loopInMs = -1;
    qint64 m_loopOutMs = -1;
    qint64 m_cuePoints[4] = {0, 0, 0, 0};
};
