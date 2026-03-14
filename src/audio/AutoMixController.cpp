#include "audio/AutoMixController.h"

AutoMixController::AutoMixController(
    DeckController* leftDeck,
    DeckController* rightDeck,
    QObject* parent
)
    : QObject(parent)
    , m_leftDeck(leftDeck)
    , m_rightDeck(rightDeck)
{
    m_timer.setInterval(200);
    connect(&m_timer, &QTimer::timeout, this, &AutoMixController::onTick);
}

void AutoMixController::setPlaylist(const QList<TrackItem>& playlist)
{
    m_playlist = playlist;
}

void AutoMixController::setCrossfadeSeconds(int seconds)
{
    m_crossfadeSeconds = qMax(2, seconds);
}

void AutoMixController::start()
{
    if (m_playlist.isEmpty())
    {
        emit statusChanged("Auto-Mix: Playlist ist leer.");
        return;
    }

    m_running = true;
    startFirstTrackIfNeeded();
    m_timer.start();
    emit statusChanged("Auto-Mix gestartet.");
}

void AutoMixController::stop()
{
    m_running = false;
    m_timer.stop();
    emit statusChanged("Auto-Mix gestoppt.");
}

bool AutoMixController::isRunning() const
{
    return m_running;
}

void AutoMixController::onTick()
{
    if (!m_running)
    {
        return;
    }

    auto* current = activeDeck();

    if (current == nullptr || current->duration() <= 0)
    {
        return;
    }

    const auto remainingMs = current->duration() - current->position();
    const auto fadeWindowMs = static_cast<qint64>(m_crossfadeSeconds) * 1000;

    if (remainingMs <= fadeWindowMs && m_currentIndex + 1 < m_playlist.size())
    {
        startNextDeck();

        const auto progress = 1.0 - (static_cast<double>(remainingMs) / static_cast<double>(fadeWindowMs));
        const auto sliderValue = qBound(0, static_cast<int>(progress * 100.0), 100);

        emit crossfaderSuggested(m_leftActive ? sliderValue : 100 - sliderValue);

        if (remainingMs <= 500)
        {
            m_leftActive = !m_leftActive;
        }
    }
}

void AutoMixController::startFirstTrackIfNeeded()
{
    if (m_currentIndex >= 0 || m_playlist.isEmpty())
    {
        return;
    }

    m_currentIndex = 0;
    emit requestLoadTrack(m_leftDeck, m_playlist.at(m_currentIndex));
}

void AutoMixController::startNextDeck()
{
    auto* nextDeck = inactiveDeck();

    if (nextDeck->isPlaying())
    {
        return;
    }

    if (m_currentIndex + 1 >= m_playlist.size())
    {
        return;
    }

    ++m_currentIndex;
    emit requestLoadTrack(nextDeck, m_playlist.at(m_currentIndex));
}

DeckController* AutoMixController::inactiveDeck() const
{
    return m_leftActive ? m_rightDeck : m_leftDeck;
}

DeckController* AutoMixController::activeDeck() const
{
    return m_leftActive ? m_leftDeck : m_rightDeck;
}
