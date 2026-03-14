#pragma once

#include <QWidget>

class QLabel;
class QSlider;

class MixerWidget : public QWidget
{
    Q_OBJECT

public:
    explicit MixerWidget(QWidget* parent = nullptr);

    int crossfaderValue() const;
    void setCrossfaderValue(int value);

signals:
    void crossfaderChanged(int value);

private:
    QSlider* m_crossfader = nullptr;
    QLabel* m_valueLabel = nullptr;
};
