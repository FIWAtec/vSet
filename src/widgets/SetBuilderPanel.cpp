#include "widgets/SetBuilderPanel.h"

#include <QHBoxLayout>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QLabel>
#include <QPlainTextEdit>
#include <QPushButton>
#include <QVBoxLayout>

SetBuilderPanel::SetBuilderPanel(QWidget* parent)
    : QWidget(parent)
{
    auto* title = new QLabel("ChatGPT Set Builder");
    title->setObjectName("SectionTitle");

    m_promptEdit = new QPlainTextEdit();
    m_promptEdit->setPlaceholderText("z. B. 45 Minuten House, warm anfangen, Peak nach 30 Minuten, saubere Übergänge");

    m_jsonEdit = new QPlainTextEdit();
    m_jsonEdit->setPlaceholderText("Hier erscheint oder importierst du JSON für das Set.");

    auto* generateButton = new QPushButton("Set mit ChatGPT erzeugen");
    generateButton->setObjectName("PrimaryButton");
    auto* importButton = new QPushButton("JSON in Playlist übernehmen");

    auto* buttonRow = new QHBoxLayout();
    buttonRow->addWidget(generateButton);
    buttonRow->addWidget(importButton);

    auto* layout = new QVBoxLayout(this);
    layout->addWidget(title);
    layout->addWidget(new QLabel("Prompt"));
    layout->addWidget(m_promptEdit, 2);
    layout->addLayout(buttonRow);
    layout->addWidget(new QLabel("Set-JSON"));
    layout->addWidget(m_jsonEdit, 3);

    connect(generateButton, &QPushButton::clicked, this, [this]()
    {
        m_openAiService.generateSetPlan(m_settings, m_promptEdit->toPlainText().trimmed(), m_localTracks);
        emit statusChanged("Set-Anfrage an ChatGPT gesendet.");
    });

    connect(importButton, &QPushButton::clicked, this, [this]()
    {
        importRawJson(m_jsonEdit->toPlainText());
    });

    connect(&m_openAiService, &OpenAiService::setPlanGenerated, this, [this](const QString& rawJson)
    {
        m_jsonEdit->setPlainText(rawJson);
        emit statusChanged("Set-JSON empfangen.");
    });

    connect(&m_openAiService, &OpenAiService::serviceError, this, [this](const QString& message)
    {
        emit statusChanged(message);
    });
}

void SetBuilderPanel::setSettings(const AppSettings& settings)
{
    m_settings = settings;
}

void SetBuilderPanel::setLocalTracks(const QList<TrackItem>& tracks)
{
    m_localTracks = tracks;
}

void SetBuilderPanel::importRawJson(const QString& rawJson)
{
    const auto doc = QJsonDocument::fromJson(rawJson.toUtf8());

    if (!doc.isObject())
    {
        emit statusChanged("Set-JSON ist ungültig.");
        return;
    }

    QList<TrackItem> items;
    const auto root = doc.object();
    const auto tracks = root.value("tracks").toArray();

    for (const auto& trackValue : tracks)
    {
        const auto obj = trackValue.toObject();

        TrackItem item;
        item.source = obj.value("source").toString();
        item.title = obj.value("title").toString();
        item.artist = obj.value("artist").toString();
        item.youtubeQuery = obj.value("query").toString();
        item.bpm = obj.value("bpm").toInt();
        item.cueMs = obj.value("cueMs").toVariant().toLongLong();
        item.loopInMs = obj.value("loopInMs").toVariant().toLongLong();
        item.loopOutMs = obj.value("loopOutMs").toVariant().toLongLong();
        item.transition = obj.value("transition").toString();

        if (item.source == "local")
        {
            item.filePath = obj.value("filePath").toString();
        }

        items.append(item);
    }

    emit setPlanImported(items);
    emit statusChanged("Set in Playlist übernommen.");
}
