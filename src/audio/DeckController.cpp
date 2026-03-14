#include "audio/DeckController.h"

#include <QFileInfo>

DeckController::DeckController(const QString& deckName, QObject* parent)
    : QObject(parent)
    , m_deckName(deckName)
{
    m_output.setVolume(0.8F);
    m_player.setAudioOutput(&m_output);

    m_loopTimer.setInterval(30);

    connect(&m_player, &QMediaPlayer::positionChanged, this, &DeckController::positionChanged);
    connect(&m_player, &QMediaPlayer::durationChanged, this, &DeckController::durationChanged);

    connect(&m_player, &QMediaPlayer::playbackStateChanged, this, [this](QMediaPlayer::PlaybackState state)
    {
        emit playbackStateChanged(state == QMediaPlayer::PlayingState);
    });

    connect(&m_loopTimer, &QTimer::timeout, this, &DeckController::onLoopTick);
}

void DeckController::loadFile(const QString& filePath)
{
    m_currentFilePath = filePath;
    m_player.setSource(QUrl::fromLocalFile(filePath));
    emit trackLoaded(filePath);
    emit statusChanged(m_deckName + ": geladen - " + QFileInfo(filePath).fileName());
}

void DeckController::play()
{
    m_player.play();
    if (m_loopInMs >= 0 && m_loopOutMs > m_loopInMs)
    {
        m_loopTimer.start();
    }
    emit statusChanged(m_deckName + ": Play");
}

void DeckController::pause()
{
    m_player.pause();
    emit statusChanged(m_deckName + ": Pause");
}

void DeckController::stop()
{
    m_player.stop();
    m_loopTimer.stop();
    emit statusChanged(m_deckName + ": Stop");
}

void DeckController::setVolume(int percent)
{
    m_userVolumePercent = qBound(0, percent, 100);
    applyVolume();
}

void DeckController::setPosition(qint64 ms)
{
    m_player.setPosition(ms);
}

void DeckController::setLoopIn()
{
    m_loopInMs = m_player.position();
    if (m_loopOutMs > m_loopInMs)
    {
        m_loopTimer.start();
    }
    emit statusChanged(m_deckName + ": Loop-In gesetzt");
}

void DeckController::setLoopOut()
{
    m_loopOutMs = m_player.position();
    if (m_loopInMs >= 0 && m_loopOutMs > m_loopInMs)
    {
        m_loopTimer.start();
    }
    emit statusChanged(m_deckName + ": Loop-Out gesetzt");
}

void DeckController::clearLoop()
{
    m_loopInMs = -1;
    m_loopOutMs = -1;
    m_loopTimer.stop();
    emit statusChanged(m_deckName + ": Loop gelöscht");
}

void DeckController::saveCue(int slot)
{
    if (slot < 0 || slot >= 4)
    {
        return;
    }

    m_cuePoints[slot] = m_player.position();
    emit statusChanged(m_deckName + ": Cue " + QString::number(slot + 1) + " gespeichert");
}

void DeckController::jumpToCue(int slot)
{
    if (slot < 0 || slot >= 4)
    {
        return;
    }

    m_player.setPosition(m_cuePoints[slot]);
    emit statusChanged(m_deckName + ": Cue " + QString::number(slot + 1) + " angesprungen");
}

void DeckController::setEffectiveCrossfadeGain(double gain)
{
    m_crossfadeGain = qBound(0.0, gain, 1.0);
    applyVolume();
}

QString DeckController::deckName() const
{
    return m_deckName;
}

QString DeckController::currentFilePath() const
{
    return m_currentFilePath;
}

qint64 DeckController::position() const
{
    return m_player.position();
}

qint64 DeckController::duration() const
{
    return m_player.duration();
}

bool DeckController::isPlaying() const
{
    return m_player.playbackState() == QMediaPlayer::PlayingState;
}

int DeckController::volumePercent() const
{
    return m_userVolumePercent;
}

void DeckController::onLoopTick()
{
    if (m_loopInMs >= 0 && m_loopOutMs > m_loopInMs && m_player.position() >= m_loopOutMs)
    {
        m_player.setPosition(m_loopInMs);
    }
}

void DeckController::applyVolume()
{
    const auto normalized = static_cast<float>(m_userVolumePercent) / 100.0F;
    m_output.setVolume(normalized * static_cast<float>(m_crossfadeGain));
}
