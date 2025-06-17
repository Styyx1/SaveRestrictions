#include "saveHandler.h"
#include "menu-event.h"

void InitListener(SKSE::MessagingInterface::Message* a_msg) {

	const auto& saveLocs = SaveHandler::SaveLocations::GetSingleton();
	const auto& eventHandler = MenuEventHandler::OpenCloseManager::GetSingleton();
	switch (a_msg->type) {
	case SKSE::MessagingInterface::kDataLoaded:		
		if (saveLocs)
			saveLocs->PopulateKeywordSets();
		MenuEventHandler::OpenCloseManager::Register();
		break;

	case SKSE::MessagingInterface::kPostLoadGame:		
		eventHandler->SetDifficultyForSaving();
		break;

	case SKSE::MessagingInterface::kNewGame:
		eventHandler->SetDifficultyForSaving();
		break;
	}
}

SKSE_PLUGIN_LOAD(const SKSE::LoadInterface* a_skse)
{
	SKSE::Init(a_skse, {.trampoline = true, .trampolineSize = 14});
	SKSE::GetMessagingInterface()->RegisterListener(InitListener);
	Config::Settings::GetSingleton()->LoadSettings();
	SaveHandler::SaveManager::GetSingleton();
	return true;
}
