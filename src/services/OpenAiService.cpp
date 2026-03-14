#include "services/OpenAiService.h"

#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkReply>
#include <QNetworkRequest>

OpenAiService::OpenAiService(QObject* parent)
    : QObject(parent)
{
}

QString OpenAiService::buildPlaylistContext(const QList<TrackItem>& localTracks) const
{
    QJsonArray trackArray;

    for (const auto& track : localTracks)
    {
        QJsonObject item;
        item["title"] = track.title;
        item["artist"] = track.artist;
        item["bpm"] = track.bpm;
        item["durationMs"] = QString::number(track.durationMs);
        item["filePath"] = track.filePath;
        trackArray.append(item);
    }

    return QString::fromUtf8(QJsonDocument(trackArray).toJson(QJsonDocument::Indented));
}

void OpenAiService::generateSetPlan(
    const AppSettings& settings,
    const QString& userPrompt,
    const QList<TrackItem>& localTracks
)
{
    if (settings.openAiApiKey.trimmed().isEmpty())
    {
        emit serviceError("OpenAI API-Key fehlt in den Einstellungen.");
        return;
    }

    QNetworkRequest request(QUrl("https://api.openai.com/v1/responses"));
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    request.setRawHeader("Authorization", "Bearer " + settings.openAiApiKey.toUtf8());

    const QString fullPrompt =
        settings.chatPromptTemplate + "\n\n"
        "Benutzerwunsch:\n" + userPrompt + "\n\n"
        "Lokale Tracks:\n" + buildPlaylistContext(localTracks) + "\n\n"
        "Antworte nur mit JSON im Format:\n"
        "{"
        "\"title\":\"...\","
        "\"targetBpm\":124,"
        "\"tracks\":["
        "{"
        "\"source\":\"local|youtube\","
        "\"title\":\"...\","
        "\"artist\":\"...\","
        "\"query\":\"...\","
        "\"bpm\":124,"
        "\"cueMs\":10000,"
        "\"loopInMs\":20000,"
        "\"loopOutMs\":28000,"
        "\"transition\":\"filter_fade\""
        "}"
        "]"
        "}";

    QJsonObject body;
    body["model"] = settings.openAiModel;

    QJsonArray input;
    QJsonObject message;
    message["role"] = "user";
    QJsonArray content;
    QJsonObject text;
    text["type"] = "input_text";
    text["text"] = fullPrompt;
    content.append(text);
    message["content"] = content;
    input.append(message);
    body["input"] = input;

    auto* reply = m_network.post(request, QJsonDocument(body).toJson(QJsonDocument::Compact));

    connect(reply, &QNetworkReply::finished, this, [this, reply]()
    {
        const auto data = reply->readAll();

        if (reply->error() != QNetworkReply::NoError)
        {
            emit serviceError("OpenAI-Antwortfehler: " + reply->errorString());
            reply->deleteLater();
            return;
        }

        const auto doc = QJsonDocument::fromJson(data);
        const auto root = doc.object();

        QString outputText;

        const auto outputArray = root.value("output").toArray();
        for (const auto& outputItem : outputArray)
        {
            const auto outputObject = outputItem.toObject();
            const auto contentArray = outputObject.value("content").toArray();

            for (const auto& contentItem : contentArray)
            {
                const auto contentObject = contentItem.toObject();
                if (contentObject.value("type").toString() == "output_text")
                {
                    outputText += contentObject.value("text").toString();
                }
            }
        }

        if (outputText.trimmed().isEmpty())
        {
            emit serviceError("OpenAI hat kein auswertbares JSON geliefert.");
        }
        else
        {
            emit setPlanGenerated(outputText);
        }

        reply->deleteLater();
    });
}
