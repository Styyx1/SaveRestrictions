#include "ui.h"

namespace UI {
	using set = Config::Settings;

	void Register() {
		if (!SKSEMenuFramework::IsInstalled()) {
			return;
		}
		SKSEMenuFramework::SetSection(MainMenuStrings::MainTitle);
		SKSEMenuFramework::AddSectionItem(MainMenuStrings::Settings, Menu::RenderSettings);
		RestoreSettingsFromConfig();
	}
	void RestoreSettingsFromConfig()
	{
		Menu::save_notify_variant_temp = static_cast<int>(set::save_notify_variant.GetValue());
		Menu::disable_save_in_combat_temp = set::disable_save_in_combat.GetValue();
		Menu::temp_save_message_temp = set::exit_save_message.GetValue();
		Menu::temp_save_time_temp = set::temporary_save_time.GetValue();
	}

	void RestoreDefaultSettings()
	{
		using namespace Menu;
		save_notify_variant_temp = 1;
		set::save_notify_variant.SetValue(save_notify_variant_temp);
		set::GetSingleton()->GetNotifyVariant();
		disable_save_in_combat_temp = true;
		set::disable_save_in_combat.SetValue(disable_save_in_combat_temp);
		temp_save_message_temp = "You can now save for {} seconds";
		set::exit_save_message.SetValue(temp_save_message_temp);
		temp_save_time_temp = 120.0f;
		set::temporary_save_time.SetValue(temp_save_time_temp);
	}

	void HelpMarker(const char* desc)
	{
		ImGui::TextDisabled("(?)");
		if (ImGui::BeginItemTooltip()) {
			ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
			ImGui::TextUnformatted(desc);
			ImGui::PopTextWrapPos();
			ImGui::EndTooltip();
		}
	}

	namespace Menu {
		using namespace Config;

		void __stdcall RenderSettings()
		{
			static std::array<const char*, 3> notify_variants = {
				"None",
				"Notification",
				"Message Box"
			};

			auto set = Settings::GetSingleton();

			// Notification Variant
			FontAwesome::PushSolid();
			ImGui::NewLine();
			ImGui::Text(MainMenuStrings::Settings.c_str());
			ImGui::SetNextItemWidth(300.0f);
			if (ImGui::Combo(Label::SaveNotifyVariantLabel.c_str(), &save_notify_variant_temp, notify_variants.data(), static_cast<int>(notify_variants.size()))) {
                    Settings::save_notify_variant.SetValue(save_notify_variant_temp);
					set->GetNotifyVariant();					
			}
			ImGui::SameLine();
			HelpMarker(Tooltip::SaveNotifyVariantLabel.c_str());

			//temp save bool
			ImGui::NewLine();
			if(ImGui::Checkbox(Label::DisableSaveInCombatLabel.c_str(), &disable_save_in_combat_temp)) {
				Settings::disable_save_in_combat.SetValue(disable_save_in_combat_temp);
			}
			ImGui::SameLine();
			HelpMarker(Tooltip::DisableSaveInCombatLabel.c_str());

			//temp save time
			ImGui::NewLine();
			ImGui::SetNextItemWidth(300.0f);
			if (ImGui::SliderFloat(Label::SaveAllowTimeLabel.c_str(), &temp_save_time_temp, 10.0f, 600.0f)) {
				Settings::temporary_save_time.SetValue(temp_save_time_temp);
			}
			ImGui::SameLine();
			HelpMarker(Tooltip::SaveAllowTimeLabel.c_str());

			ImGui::NewLine();
			ImGui::SetNextItemWidth(400.0f);
			char buffer[256];
			std::strncpy(buffer, temp_save_message_temp.c_str(), sizeof(buffer));
			if (ImGui::InputText(Label::SaveMessageLabel.c_str(), buffer, sizeof(buffer))) {
				temp_save_message_temp = std::string(buffer);
				Settings::exit_save_message.SetValue(temp_save_message_temp);
			}
			ImGui::SameLine();
			HelpMarker(Tooltip::SaveMessageLabel.c_str());

			//Save Config and Default Settings
			ImGui::NewLine();
			ImGui::Text(MainMenuStrings::System.c_str());
			if (ImGui::Button(MainMenuStrings::Save.c_str())) {
				Settings::GetSingleton()->UpdateSettings(true);
			}
			ImGui::SameLine();
			if (ImGui::Button(MainMenuStrings::Restore.c_str())) {
				RestoreDefaultSettings();
			}


			FontAwesome::Pop();
		}
	}
}