#pragma once

#include "services/YouTubeService.h"
#include "model/TrackItem.h"

#include <QWidget>

class QListWidget;
class QLineEdit;
class QPushButton;
QT_BEGIN_NAMESPACE
class QWebEngineView;
QT_END_NAMESPACE

class YouTubePanel : public QWidget
{
    Q_OBJECT

public:
    explicit YouTubePanel(QWidget* parent = nullptr);

    void setApiKey(const QString& apiKey);
    void search(const QString& query);

signals:
    void addYouTubeTrackRequested(const TrackItem& track);
    void statusChanged(const QString& message);

private:
    QString m_apiKey;
    YouTubeService m_service;
    QLineEdit* m_searchEdit = nullptr;
    QListWidget* m_resultsList = nullptr;
    QWebEngineView* m_webView = nullptr;
    QList<YouTubeVideo> m_results;
};
