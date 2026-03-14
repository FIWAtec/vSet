#pragma once

#include "model/TrackItem.h"
#include "services/OpenAiService.h"
#include "services/SettingsService.h"

#include <QWidget>

class QPlainTextEdit;
class QPushButton;

class SetBuilderPanel : public QWidget
{
    Q_OBJECT

public:
    explicit SetBuilderPanel(QWidget* parent = nullptr);

    void setSettings(const AppSettings& settings);
    void setLocalTracks(const QList<TrackItem>& tracks);

signals:
    void setPlanImported(const QList<TrackItem>& tracks);
    void statusChanged(const QString& message);

private:
    void importRawJson(const QString& rawJson);

    AppSettings m_settings;
    QList<TrackItem> m_localTracks;
    OpenAiService m_openAiService;
    QPlainTextEdit* m_promptEdit = nullptr;
    QPlainTextEdit* m_jsonEdit = nullptr;
};
