#include "menu-event.h"
#include "saveHandler.h"
#include "ui.h"

namespace MenuEventHandler{
	void OpenCloseManager::Register()
	{
		RE::UI::GetSingleton()->AddEventSink(GetSingleton());
		REX::INFO("Registered for {}", typeid(OpenCloseManager).name());
	}

	void OpenCloseManager::SetDifficultyForSaving()
	{
		const auto& saveHandler = SaveHandler::SaveLocations::GetSingleton();
		const auto& player = RE::PlayerCharacter::GetSingleton();
		if (player->difficulty != _storedDifficulty) {
			saveHandler->SetSaveAllowLocationDifficulty(saveHandler->GetDifficultyFromGame(player));
			_storedDifficulty = player->difficulty;
		}		
	}

	RE::BSEventNotifyControl OpenCloseManager::ProcessEvent(const RE::MenuOpenCloseEvent* event, RE::BSTEventSource<RE::MenuOpenCloseEvent>* a_eventSource)
	{
		if(!event)
			return RE::BSEventNotifyControl::kContinue;

		if(event->menuName != RE::JournalMenu::MENU_NAME)
			return RE::BSEventNotifyControl::kContinue;

		if(event->opening)
			return RE::BSEventNotifyControl::kContinue;

		SetDifficultyForSaving();
		const auto& saveManager = SaveHandler::SaveManager::GetSingleton();
		saveManager->UpdateSaveEnableState();

		return RE::BSEventNotifyControl::kContinue;
	}
}