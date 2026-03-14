#pragma once

#include <QString>

struct AppSettings
{
    QString openAiApiKey;
    QString openAiModel = "gpt-4.1-mini";
    QString youtubeApiKey;
    QString chatPromptTemplate;
    int defaultAutoMixSeconds = 10;
};

class SettingsService
{
public:
    static AppSettings load();
    static void save(const AppSettings& settings);
};
