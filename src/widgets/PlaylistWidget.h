#pragma once

#include "model/TrackItem.h"

#include <QWidget>

class QListWidget;
class QPushButton;

class PlaylistWidget : public QWidget
{
    Q_OBJECT

public:
    explicit PlaylistWidget(QWidget* parent = nullptr);

    QList<TrackItem> tracks() const;
    void setTracks(const QList<TrackItem>& tracks);
    void appendTrack(const TrackItem& track);

signals:
    void tracksChanged(const QList<TrackItem>& tracks);

private:
    QListWidget* m_list = nullptr;
    QList<TrackItem> m_tracks;

    void refresh();
};
