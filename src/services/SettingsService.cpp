#include "services/SettingsService.h"

#include <QSettings>

AppSettings SettingsService::load()
{
    QSettings settings("FIWAtec", "VirtualDjSetPro");

    AppSettings result;
    result.openAiApiKey = settings.value("openai/apiKey").toString();
    result.openAiModel = settings.value("openai/model", "gpt-4.1-mini").toString();
    result.youtubeApiKey = settings.value("youtube/apiKey").toString();
    result.defaultAutoMixSeconds = settings.value("mix/defaultSeconds", 10).toInt();
    result.chatPromptTemplate = settings.value(
        "chat/promptTemplate",
        "Erstelle ein DJ-Set als JSON. Nutze cues, loops, Übergänge und sinnvolle Reihenfolge. "
        "Wenn du YouTube-Titel vorschlägst, liefere nur Suchqueries statt direkter Streams."
    ).toString();

    return result;
}

void SettingsService::save(const AppSettings& settings)
{
    QSettings store("FIWAtec", "VirtualDjSetPro");
    store.setValue("openai/apiKey", settings.openAiApiKey);
    store.setValue("openai/model", settings.openAiModel);
    store.setValue("youtube/apiKey", settings.youtubeApiKey);
    store.setValue("mix/defaultSeconds", settings.defaultAutoMixSeconds);
    store.setValue("chat/promptTemplate", settings.chatPromptTemplate);
    store.sync();
}
