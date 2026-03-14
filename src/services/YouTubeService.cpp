#include "services/YouTubeService.h"

#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QUrlQuery>

YouTubeService::YouTubeService(QObject* parent)
    : QObject(parent)
{
}

void YouTubeService::search(const QString& apiKey, const QString& query)
{
    if (apiKey.trimmed().isEmpty())
    {
        emit serviceError("YouTube API-Key fehlt in den Einstellungen.");
        return;
    }

    QUrl url("https://www.googleapis.com/youtube/v3/search");
    QUrlQuery urlQuery;
    urlQuery.addQueryItem("part", "snippet");
    urlQuery.addQueryItem("type", "video");
    urlQuery.addQueryItem("maxResults", "15");
    urlQuery.addQueryItem("q", query);
    urlQuery.addQueryItem("key", apiKey);
    url.setQuery(urlQuery);

    QNetworkRequest request(url);
    auto* reply = m_network.get(request);

    connect(reply, &QNetworkReply::finished, this, [this, reply]()
    {
        const auto data = reply->readAll();

        if (reply->error() != QNetworkReply::NoError)
        {
            emit serviceError("YouTube-Suche fehlgeschlagen: " + reply->errorString());
            reply->deleteLater();
            return;
        }

        QList<YouTubeVideo> results;
        const auto doc = QJsonDocument::fromJson(data);
        const auto items = doc.object().value("items").toArray();

        for (const auto& item : items)
        {
            const auto obj = item.toObject();
            const auto idObj = obj.value("id").toObject();
            const auto snippet = obj.value("snippet").toObject();

            YouTubeVideo video;
            video.videoId = idObj.value("videoId").toString();
            video.title = snippet.value("title").toString();
            video.channelTitle = snippet.value("channelTitle").toString();
            results.append(video);
        }

        emit searchReady(results);
        reply->deleteLater();
    });
}
