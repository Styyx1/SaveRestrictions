#pragma once

namespace Config {

	enum class NotificationVariant {
		kNone = 0,
		kNotification = 1,
		kMessageBox = 2
	};

	class Settings : public REX::Singleton<Settings>
	{
	public:
		static inline REX::TOML::I32 save_notify_variant{ "Saving.Toggles", "iSaveNotificationVariant", 2};
		static inline REX::TOML::Str exit_save_message{ "Saving.Texts", "sTemporarySaveMessageBox", std::string("You can now save for  {} seconds") };
		static inline REX::TOML::Str save_file_name{ "Saving.Texts", "sSaveFileName", std::string("SaveRestrictionsExitSave") };
		static inline REX::TOML::F32 temporary_save_time{ "Saving.Settings", "fTemporarySaveEnableTime", 120.0f };

		NotificationVariant GetNotifyVariant() const noexcept {
			return static_cast<NotificationVariant>(save_notify_variant.GetValue());
		}

		void LoadSettings() {
			const auto toml = REX::TOML::SettingStore::GetSingleton();
			toml->Init("Data/skse/plugins/saveRestrictions.toml", "Data/skse/plugins/saveRestrictions_custom.toml");
			toml->Load();
		}
	};
}

