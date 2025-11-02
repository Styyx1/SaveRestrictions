#pragma once

#include "mod-data.h"

namespace Config {

	enum class NotificationVariant {
		kNone = 0,
		kNotification = 1,
		kMessageBox = 2
	};

	inline const char* ToString(NotificationVariant variant)
	{
		switch (variant) {
		case NotificationVariant::kNone:         return "None";
		case NotificationVariant::kNotification: return "Notification";
		case NotificationVariant::kMessageBox:   return "Message Box";
		default:                                 return "Unknown";
		}
	};

	inline const std::pair<NotificationVariant, const char*> variantNames[] = {
	{ NotificationVariant::kNone, "None" },
	{ NotificationVariant::kNotification, "Notification" },
	{ NotificationVariant::kMessageBox, "Message Box" }
	};

	using namespace MOD;
	class Settings : public REX::Singleton<Settings>
	{
		
	public:
		static inline REX::TOML::I32 save_notify_variant{ "Saving.Toggles", "iSaveNotificationVariant", 2};
		static inline REX::TOML::Bool disable_save_in_combat{ "Saving.Toggles", "bDisableSavesInCombat", true };
		static inline REX::TOML::Str exit_save_message{ "Saving.Texts", "sTemporarySaveMessageBox", std::string("You can now save for {} seconds") };
		static inline REX::TOML::F32 temporary_save_time{ "Saving.Settings", "fTemporarySaveEnableTime", 120.0f };

		NotificationVariant GetNotifyVariant() const noexcept {
			return static_cast<NotificationVariant>(save_notify_variant.GetValue());
		}

		inline void UpdateSettings(bool save = false) const {
			const auto toml = REX::TOML::SettingStore::GetSingleton();
			toml->Init(TOML_PATH_DEFAULT.data(), TOML_PATH_CUSTOM.data());
			if (!save)
				toml->Load();
			else
				toml->Save();

			GetNotifyVariant();
		}
	};
}