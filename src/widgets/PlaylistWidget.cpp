#include "widgets/PlaylistWidget.h"

#include <QFileDialog>
#include <QFileInfo>
#include <QHBoxLayout>
#include <QLabel>
#include <QListWidget>
#include <QPushButton>
#include <QVBoxLayout>

PlaylistWidget::PlaylistWidget(QWidget* parent)
    : QWidget(parent)
{
    auto* title = new QLabel("Playlist");
    title->setObjectName("SectionTitle");

    auto* importButton = new QPushButton("Tracks importieren");
    auto* clearButton = new QPushButton("Playlist leeren");
    auto* buttonRow = new QHBoxLayout();
    buttonRow->addWidget(importButton);
    buttonRow->addWidget(clearButton);

    m_list = new QListWidget();

    auto* layout = new QVBoxLayout(this);
    layout->addWidget(title);
    layout->addLayout(buttonRow);
    layout->addWidget(m_list);

    connect(importButton, &QPushButton::clicked, this, [this]()
    {
        const auto paths = QFileDialog::getOpenFileNames(
            this,
            "Audio-Dateien importieren",
            QString(),
            "Audio (*.mp3 *.wav *.flac *.ogg *.m4a)"
        );

        for (const auto& path : paths)
        {
            TrackItem item;
            item.source = "local";
            item.filePath = path;
            item.title = QFileInfo(path).baseName();
            item.artist = "Unknown";
            appendTrack(item);
        }
    });

    connect(clearButton, &QPushButton::clicked, this, [this]()
    {
        m_tracks.clear();
        refresh();
        emit tracksChanged(m_tracks);
    });
}

QList<TrackItem> PlaylistWidget::tracks() const
{
    return m_tracks;
}

void PlaylistWidget::setTracks(const QList<TrackItem>& tracks)
{
    m_tracks = tracks;
    refresh();
    emit tracksChanged(m_tracks);
}

void PlaylistWidget::appendTrack(const TrackItem& track)
{
    m_tracks.append(track);
    refresh();
    emit tracksChanged(m_tracks);
}

void PlaylistWidget::refresh()
{
    m_list->clear();

    for (const auto& track : m_tracks)
    {
        const auto label = track.source == "youtube"
            ? "[YouTube] " + (track.youtubeQuery.isEmpty() ? track.title : track.youtubeQuery)
            : "[Local] " + track.title;

        m_list->addItem(label);
    }
}
