#include "saveHandler.h"

namespace SaveHandler {

	void SaveLocations::SetSaveAllowLocationDifficulty(GameDiff a_difficulty)
	{
		switch (a_difficulty.get()) {
		case GameDifficulty::Novice:
			SetSaveRestrictions(SaveAllowLoc::kSaveSpot);
			break;
		case GameDifficulty::Apprentice:
			SetSaveRestrictions(SaveAllowLoc::kNonDangerZones);
			break;
		case GameDifficulty::Adept:
			SetSaveRestrictions(SaveAllowLoc::kAllExteriors);
			break;
		case GameDifficulty::Expert:
			SetSaveRestrictions(SaveAllowLoc::kCitiesTowns);
			break;
		case GameDifficulty::Master:
			SetSaveRestrictions(SaveAllowLoc::kOnlyInnsAndCities);
			break;
		case GameDifficulty::Legendary:
			SetSaveRestrictions(SaveAllowLoc::kOnlyInns);
			break;
		default:
			SetSaveRestrictions(SaveAllowLoc::kSaveSpot);
			break;
		}
	}

	SaveLocations::GameDiff SaveLocations::GetDifficultyFromGame(RE::PlayerCharacter* a_player)
	{
		switch (a_player->difficulty) {
		case 0:
			return GameDiff::enum_type::Novice;

		case 1:
			return GameDiff::enum_type::Apprentice;

		case 2:
			return GameDiff::enum_type::Adept;

		case 3:
			return GameDiff::enum_type::Expert;

		case 4:
			return GameDiff::enum_type::Master;

		case 5:
			return GameDiff::enum_type::Legendary;

		default:
			return GameDiff::enum_type::Adept;
		}
	}

	void SaveLocations::PopulateKeywordSets()
	{
		// Inns
		inns = {
			"LocTypeInn",
			"LocTypeTemple",
			"LocTypePlayerHouse",
		};

		// Towns
		towns = {
			"LocTypeTown",
			"LocTypeSettlement",
		};

		// Cities
		cities = {
			"LocTypeCity",
			"LocTypeHoldCapital",
		};

		// Non-danger zones
		nonDangerZones = {
			"LocTypeFarm",
			"LocTypeHabitation",
			"LocTypeStore",
			"LocTypeLumberMill",
			"LocTypeTemple",
			"LocTypeDwelling",
			"LocTypeGuild",
			"LocTypeOrcStronghold",
			"LocTypeStewardsDwelling",
			"LocTypePlayerHouse",
			"LocTypeHouse",
			"LocTypeJail",
			"LocTypeShip",
			"LocTypeHold"
		};

		// Danger zones
		dangerZones = {
			"LocTypeAnimalDen",
			"LocTypeBanditCamp",
			"LocTypeCastle",
			"LocTypeCemetery",
			"LocTypeDragonLair",
			"LocTypeDragonPriestLair",
			"LocTypeDraugrCrypt",
			"LocTypeDungeon",
			"LocTypeDwarvenAutomatons",
			"LocTypeFalmerHive",
			"LocTypeForswornCamp",
			"LocTypeGiantCamp",
			"LocTypeHagravenNest",
			"LocTypeMine",
			"LocTypeSprigganGrove",
			"LocTypeVampireLair",
			"LocTypeWarlockLair",
			"LocTypeWerewolfLair"
		};
	}

	bool SaveManager::GetIsInSaveAllowLoc() const
	{
		if (SaveTimer::GetSingleton()->IsRunning()) {
			return true;
		}

		auto player = RE::PlayerCharacter::GetSingleton();
		if (!player)
			return false;

		auto playerCell = player->GetParentCell();
		if (!playerCell) {
			playerCell = player->GetSaveParentCell();
			if (!playerCell)
				return false;
		}

		auto saveLocs = SaveLocations::GetSingleton();
		if (!saveLocs)
			return false;

		auto saveRestriction = saveLocs->GetSaveRestrictions().get();
		auto playerLoc = player->GetCurrentLocation();

		bool result = false;

		switch (saveRestriction) {
		case SaveLocations::SaveAllowLocEnum::enum_type::kSaveSpot:
			result = true;
			break;

		case SaveLocations::SaveAllowLocEnum::enum_type::kAllExteriors:
			result = !playerCell->IsInteriorCell();
			break;

		case SaveLocations::SaveAllowLocEnum::enum_type::kNonDangerZones:
			result = playerLoc && !LocationHasAnyKeyword(playerLoc, saveLocs->dangerZones);
			result = result || !playerCell->IsInteriorCell();
			break;

		case SaveLocations::SaveAllowLocEnum::enum_type::kCitiesTowns:
			if (playerLoc)
				result = LocationHasAnyKeyword(playerLoc, saveLocs->cities) || LocationHasAnyKeyword(playerLoc, saveLocs->towns);
			break;

		case SaveLocations::SaveAllowLocEnum::enum_type::kOnlyInnsAndCities:
			if (playerLoc)
				result = LocationHasAnyKeyword(playerLoc, saveLocs->cities) || LocationHasAnyKeyword(playerLoc, saveLocs->inns);
			break;

		case SaveLocations::SaveAllowLocEnum::enum_type::kOnlyInns:
			if (playerLoc)
				result = LocationHasAnyKeyword(playerLoc, saveLocs->inns);
			break;

		default:
			result = true;
			break;
		}
		REX::DEBUG("result in function is {}", result ? "true" : "false");
		return result;
	}

	void SaveManager::UpdateSaveEnableState() const
	{
		const bool isInAllowedLocation = GetIsInSaveAllowLoc();
		const auto& saveLocs = SaveLocations::GetSingleton();

		saveLocs->UpdateSaveState(isInAllowedLocation
			? SaveLocations::SaveEnableStateEnum::enum_type::kEnabled
			: SaveLocations::SaveEnableStateEnum::enum_type::kDisabled);
	}

	void SaveManager::PlayerUpdate(RE::PlayerCharacter* a_player, float a_delta)
	{
		const auto& self = SaveManager::GetSingleton();
		const auto& saveTimer = SaveTimer::GetSingleton();
		if (saveTimer->IsRunning() && saveTimer->Elapsed() >= saveTimer->GetExpectedRuntime()) {
			REX::DEBUG("resetting timer, expected runtime was: {}, actual runtime is: {}", saveTimer->GetExpectedRuntime(), saveTimer->Elapsed());
			saveTimer->Stop();
			SaveItems::GetSingleton()->SetIsWorking(false);
		}		

		const bool isAllowed = self->GetIsInSaveAllowLoc();
		REX::DEBUG("isAllowed in main loop is {}", isAllowed ? "true" : "false");

		const auto& saveLocs = SaveHandler::SaveLocations::GetSingleton();
		saveLocs->UpdateSaveState(isAllowed
			? SaveLocations::SaveEnableStateEnum::enum_type::kEnabled
			: SaveLocations::SaveEnableStateEnum::enum_type::kDisabled);

		if (saveLocs->GetSaveState() == SaveLocations::SaveEnableState::kEnabled) {
			AllowSaving();
		}
		else {
			PreventSaving();
		}
		_Hook0(a_player, a_delta);
	};


	bool SaveManager::LocationHasAnyKeyword(RE::BGSLocation* a_location, const std::unordered_set<std::string_view>& keywords) const
	{
		if (!a_location)
			return false;

		for (const auto& keyword : keywords) {
			if (a_location->HasKeywordString(keyword)) {
				return true;
			}
		}
		return false;
	}

	RE::UI_MESSAGE_RESULTS SaveItems::ProcessMessage(RE::BookMenu* a_this, RE::UIMessage& a_message)
	{
		auto book = a_this->GetTargetForm();
		if (a_message.type == RE::UI_MESSAGE_TYPE::kHide) {
			if (book && book->HasKeywordString("SavingBook") && !_inProgress) {
				SaveItems::GetSingleton()->SetIsWorking(true);
				const auto& settings = Config::Settings::GetSingleton();
				auto seconds = settings->temporary_save_time.GetValue();
				SaveTimer::GetSingleton()->Start(seconds);			
				try {
					auto msgTemplate = settings->exit_save_message.GetValue();
					if (!msgTemplate.empty()) {
						std::string finalMessage = std::vformat(msgTemplate, std::make_format_args(seconds));
						ShowSaveNotification(finalMessage);
					}
					else {
						ShowSaveNotification("Temporary saving enabled");
					}
				}
				catch (const std::format_error& e) {
					RE::DebugNotification("Temporary saving enabled.", nullptr, true);
				}							
			}
		}
		return _Hook1(a_this, a_message);
	}
	void SaveItems::ShowSaveNotification(const std::string& message)
	{
		const auto settings = Config::Settings::GetSingleton();
		switch (settings->GetNotifyVariant()) {
		case Config::NotificationVariant::kMessageBox:
			RE::DebugMessageBox(message.c_str());
			break;
		case Config::NotificationVariant::kNotification:
			RE::DebugNotification(message.c_str(), nullptr, true);
			break;
		case Config::NotificationVariant::kNone:
		default:
			break;
		}
	}
}