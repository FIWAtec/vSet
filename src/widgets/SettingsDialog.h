#pragma once

#include "services/SettingsService.h"

#include <QDialog>

class QLineEdit;
class QPlainTextEdit;
class QSpinBox;

class SettingsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SettingsDialog(const AppSettings& settings, QWidget* parent = nullptr);
    AppSettings settings() const;

private:
    QLineEdit* m_openAiApiKey = nullptr;
    QLineEdit* m_openAiModel = nullptr;
    QLineEdit* m_youtubeApiKey = nullptr;
    QPlainTextEdit* m_promptTemplate = nullptr;
    QSpinBox* m_autoMixSeconds = nullptr;
};
