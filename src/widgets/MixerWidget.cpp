#include "widgets/MixerWidget.h"

#include <QHBoxLayout>
#include <QLabel>
#include <QSlider>
#include <QVBoxLayout>

MixerWidget::MixerWidget(QWidget* parent)
    : QWidget(parent)
{
    auto* title = new QLabel("Mixer");
    title->setObjectName("SectionTitle");

    m_crossfader = new QSlider(Qt::Horizontal);
    m_crossfader->setRange(0, 100);
    m_crossfader->setValue(50);

    m_valueLabel = new QLabel("Crossfader: 50");

    auto* layout = new QVBoxLayout(this);
    layout->addWidget(title);
    layout->addWidget(m_crossfader);
    layout->addWidget(m_valueLabel);
    layout->addStretch();

    connect(m_crossfader, &QSlider::valueChanged, this, [this](int value)
    {
        m_valueLabel->setText("Crossfader: " + QString::number(value));
        emit crossfaderChanged(value);
    });
}

int MixerWidget::crossfaderValue() const
{
    return m_crossfader->value();
}

void MixerWidget::setCrossfaderValue(int value)
{
    m_crossfader->setValue(value);
}
