#pragma once

#include "services/SettingsService.h"

#include <QObject>
#include <QNetworkAccessManager>

struct YouTubeVideo
{
    QString videoId;
    QString title;
    QString channelTitle;
};

class YouTubeService : public QObject
{
    Q_OBJECT

public:
    explicit YouTubeService(QObject* parent = nullptr);
    void search(const QString& apiKey, const QString& query);

signals:
    void searchReady(const QList<YouTubeVideo>& videos);
    void serviceError(const QString& message);

private:
    QNetworkAccessManager m_network;
};
