#include "widgets/SettingsDialog.h"

#include <QDialogButtonBox>
#include <QFormLayout>
#include <QLineEdit>
#include <QPlainTextEdit>
#include <QSpinBox>
#include <QVBoxLayout>

SettingsDialog::SettingsDialog(const AppSettings& settings, QWidget* parent)
    : QDialog(parent)
{
    setWindowTitle("Einstellungen");
    resize(900, 650);

    m_openAiApiKey = new QLineEdit(settings.openAiApiKey);
    m_openAiModel = new QLineEdit(settings.openAiModel);
    m_youtubeApiKey = new QLineEdit(settings.youtubeApiKey);
    m_promptTemplate = new QPlainTextEdit(settings.chatPromptTemplate);
    m_autoMixSeconds = new QSpinBox();
    m_autoMixSeconds->setRange(2, 60);
    m_autoMixSeconds->setValue(settings.defaultAutoMixSeconds);

    auto* form = new QFormLayout();
    form->addRow("OpenAI API-Key", m_openAiApiKey);
    form->addRow("OpenAI Modell", m_openAiModel);
    form->addRow("YouTube API-Key", m_youtubeApiKey);
    form->addRow("Standard Auto-Mix Sekunden", m_autoMixSeconds);
    form->addRow("Prompt-Template für Set-Erstellung", m_promptTemplate);

    auto* buttons = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);

    auto* layout = new QVBoxLayout(this);
    layout->addLayout(form);
    layout->addWidget(buttons);

    connect(buttons, &QDialogButtonBox::accepted, this, &QDialog::accept);
    connect(buttons, &QDialogButtonBox::rejected, this, &QDialog::reject);
}

AppSettings SettingsDialog::settings() const
{
    AppSettings value;
    value.openAiApiKey = m_openAiApiKey->text().trimmed();
    value.openAiModel = m_openAiModel->text().trimmed();
    value.youtubeApiKey = m_youtubeApiKey->text().trimmed();
    value.chatPromptTemplate = m_promptTemplate->toPlainText().trimmed();
    value.defaultAutoMixSeconds = m_autoMixSeconds->value();
    return value;
}
