#pragma once

#include <QString>
#include <QUrl>

struct TrackItem
{
    QString source;
    QString title;
    QString artist;
    QString filePath;
    QString youtubeVideoId;
    QString youtubeQuery;
    int bpm = 0;
    qint64 durationMs = 0;
    qint64 cueMs = 0;
    qint64 loopInMs = -1;
    qint64 loopOutMs = -1;
    QString transition;
};
