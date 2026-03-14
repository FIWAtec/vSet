#pragma once

#include "model/SetPlan.h"
#include "services/SettingsService.h"

#include <QObject>
#include <QNetworkAccessManager>

class OpenAiService : public QObject
{
    Q_OBJECT

public:
    explicit OpenAiService(QObject* parent = nullptr);

    void generateSetPlan(
        const AppSettings& settings,
        const QString& userPrompt,
        const QList<TrackItem>& localTracks
    );

signals:
    void setPlanGenerated(const QString& rawJson);
    void serviceError(const QString& message);

private:
    QString buildPlaylistContext(const QList<TrackItem>& localTracks) const;
    QNetworkAccessManager m_network;
};
