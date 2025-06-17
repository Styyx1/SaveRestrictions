#pragma once
#include "settings.h"

namespace SaveHandler {

	class SaveLocations : public REX::Singleton<SaveLocations>{
	public:
		enum class SaveEnableState : uint8_t {			
			kDisabled = 0,
			kEnabled = 1,
		};

		enum class SaveAllowLoc : uint8_t {
			kSaveSpot = 0,
			kNonDangerZones = 1,
			kAllExteriors = 2,
			kCitiesTowns = 3,
			kOnlyInnsAndCities = 4,
			kOnlyInns = 5,
		};

		enum class GameDifficulty : uint8_t {
			Novice = 0,
			Apprentice = 1,
			Adept = 2,
			Expert = 3,
			Master = 4,
			Legendary = 5
		};

		using SaveEnableStateEnum = REX::Enum<SaveEnableState>;
		using SaveAllowLocEnum    = REX::Enum<SaveAllowLoc>;
		using GameDiff = REX::Enum<GameDifficulty>;

		[[nodiscard]] SaveEnableStateEnum GetSaveState() const { return _saveState; }
		void UpdateSaveState(SaveEnableStateEnum a_state)      { _saveState = a_state; }	

		[[nodiscard]] SaveAllowLocEnum GetSaveRestrictions() const { return _saveRestriction; }
		void SetSaveRestrictions(SaveAllowLocEnum a_restriction)   { _saveRestriction = a_restriction; }

		void SetSaveAllowLocationDifficulty(GameDiff a_difficulty);
		GameDiff GetDifficultyFromGame(RE::PlayerCharacter* a_player);

		std::unordered_set<std::string_view> inns;
		std::unordered_set<std::string_view> towns;
		std::unordered_set<std::string_view> cities;
		std::unordered_set<std::string_view> nonDangerZones;
		std::unordered_set<std::string_view> dangerZones;
		void PopulateKeywordSets();

	private:
		SaveEnableStateEnum _saveState{ SaveEnableState::kEnabled };
		SaveAllowLocEnum    _saveRestriction{ SaveAllowLoc::kSaveSpot };
	};

	class SaveItems : public REX::Singleton<SaveItems>
	{
	public:
		static RE::UI_MESSAGE_RESULTS ProcessMessage(RE::BookMenu* a_this, RE::UIMessage& a_message);
		void SetIsWorking(bool set) { _inProgress = set; };

	private:
		static inline REL::HookVFT _Hook2{ RE::VTABLE_BookMenu[0], 0x4, ProcessMessage };
		static inline bool _inProgress{ false };
		static void ShowSaveNotification(const std::string& message);
	};

	class SaveTimer : public REX::Singleton<SaveTimer>
	{
	public:
		void Start(float expectedRuntime)
		{
			startTime = std::chrono::steady_clock::now();
			_expectedRuntime = expectedRuntime;
			_isRunning = true;
		}

		void Stop()
		{
			_isRunning = false;
		}

		void Reset()
		{
			startTime = std::chrono::steady_clock::now();
		}

		[[nodiscard]] float Elapsed() const
		{
			if (!_isRunning)
				return 0.0f;

			auto now = std::chrono::steady_clock::now();
			auto dur = std::chrono::duration_cast<std::chrono::milliseconds>(now - startTime).count();
			return dur / 1000.0f;
		}

		[[nodiscard]] bool IsRunning() const
		{
			return _isRunning;
		}

		[[nodiscard]] float GetExpectedRuntime() const
		{
			return _expectedRuntime;
		}

	private:
		std::chrono::steady_clock::time_point startTime{};
		bool _isRunning{ false };
		float _expectedRuntime{ 0.0f };
	};

	class SaveManager : public REX::Singleton<SaveManager>
	{
	public:
		bool GetIsInSaveAllowLoc() const;
		void UpdateSaveEnableState() const;
		static void PreventSaving() {
			RE::PlayerCharacter::GetSingleton()->byCharGenFlag.set(RE::PlayerCharacter::ByCharGenFlag::kDisableSaving);
		}
		static void AllowSaving() {

			RE::PlayerCharacter::GetSingleton()->byCharGenFlag.reset(RE::PlayerCharacter::ByCharGenFlag::kDisableSaving);
			
		}
		static void PlayerUpdate(RE::PlayerCharacter* a_player, float a_delta);

	private:
		static inline REL::HookVFT _Hook0 { RE::VTABLE_PlayerCharacter[0], 0xAD, &PlayerUpdate };
		bool LocationHasAnyKeyword(RE::BGSLocation* a_location, const std::unordered_set<std::string_view>& keywords) const;		
	};	
}