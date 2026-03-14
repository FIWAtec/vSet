#include "widgets/YouTubePanel.h"

#include "model/TrackItem.h"

#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QListWidget>
#include <QPushButton>
#include <QUrl>
#include <QVBoxLayout>
#include <QtWebEngineWidgets/QWebEngineView>

YouTubePanel::YouTubePanel(QWidget* parent)
    : QWidget(parent)
{
    auto* title = new QLabel("YouTube Suche / Preview");
    title->setObjectName("SectionTitle");

    m_searchEdit = new QLineEdit();
    m_searchEdit->setPlaceholderText("YouTube-Track oder Mix suchen");

    auto* searchButton = new QPushButton("Suchen");
    auto* addButton = new QPushButton("Auswahl zur Playlist");
    m_resultsList = new QListWidget();
    m_webView = new QWebEngineView();

    auto* searchRow = new QHBoxLayout();
    searchRow->addWidget(m_searchEdit);
    searchRow->addWidget(searchButton);
    searchRow->addWidget(addButton);

    auto* layout = new QVBoxLayout(this);
    layout->addWidget(title);
    layout->addLayout(searchRow);
    layout->addWidget(m_resultsList, 2);
    layout->addWidget(m_webView, 4);

    connect(searchButton, &QPushButton::clicked, this, [this]()
    {
        search(m_searchEdit->text().trimmed());
    });

    connect(&m_service, &YouTubeService::searchReady, this, [this](const QList<YouTubeVideo>& videos)
    {
        m_results = videos;
        m_resultsList->clear();

        for (const auto& video : videos)
        {
            m_resultsList->addItem(video.title + " — " + video.channelTitle);
        }

        emit statusChanged("YouTube-Suche abgeschlossen.");
    });

    connect(&m_service, &YouTubeService::serviceError, this, [this](const QString& message)
    {
        emit statusChanged(message);
    });

    connect(m_resultsList, &QListWidget::currentRowChanged, this, [this](int row)
    {
        if (row < 0 || row >= m_results.size())
        {
            return;
        }

        const auto& video = m_results.at(row);
        const auto embedUrl = QString("https://www.youtube.com/embed/%1").arg(video.videoId);
        m_webView->setUrl(QUrl(embedUrl));
    });

    connect(addButton, &QPushButton::clicked, this, [this]()
    {
        const auto row = m_resultsList->currentRow();
        if (row < 0 || row >= m_results.size())
        {
            return;
        }

        const auto& video = m_results.at(row);

        TrackItem item;
        item.source = "youtube";
        item.title = video.title;
        item.artist = video.channelTitle;
        item.youtubeVideoId = video.videoId;
        item.youtubeQuery = video.title + " " + video.channelTitle;

        emit addYouTubeTrackRequested(item);
        emit statusChanged("YouTube-Titel zur Playlist hinzugefügt.");
    });
}

void YouTubePanel::setApiKey(const QString& apiKey)
{
    m_apiKey = apiKey;
}

void YouTubePanel::search(const QString& query)
{
    if (query.trimmed().isEmpty())
    {
        emit statusChanged("Bitte Suchtext eingeben.");
        return;
    }

    m_service.search(m_apiKey, query);
}
