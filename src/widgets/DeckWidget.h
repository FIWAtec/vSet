#pragma once

#include "audio/DeckController.h"

#include <QWidget>

class QLabel;
class QPushButton;
class QSlider;

class DeckWidget : public QWidget
{
    Q_OBJECT

public:
    explicit DeckWidget(DeckController* controller, QWidget* parent = nullptr);

private:
    DeckController* m_controller = nullptr;
    QLabel* m_trackLabel = nullptr;
    QLabel* m_timeLabel = nullptr;
    QSlider* m_positionSlider = nullptr;
    QSlider* m_volumeSlider = nullptr;

    void updateTimeLabel(qint64 position, qint64 duration);
};
