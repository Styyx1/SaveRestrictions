#pragma once
#include <API/SKSEMenuFramework.h>
#include "settings.h"

namespace UI {
    using str = std::string;
    namespace MainMenuStrings {
        inline str MainTitle = "Save Restrictions";
        inline str Settings = "Settings";
        inline str Toggles = "Toggles";
        inline str Save = FontAwesome::UnicodeToUtf8(0xf0c7) + " Save Settings";
        inline str Restore = FontAwesome::UnicodeToUtf8(0xf0e2) + " Restore Default Settings";
        inline str System = FontAwesome::UnicodeToUtf8(0xf390) + " System";
        inline str CurrentlyRestricted = "Is Saving Restricted?";
        //inline str HideNPCTitle = FontAwesome::UnicodeToUtf8(0xf070) + " NPC Fade Settings";
    }

    void Register();

    void RestoreSettingsFromConfig();
    void RestoreDefaultSettings();
    void HelpMarker(const char* desc);

    namespace Menu {
        void __stdcall RenderSettings();
        namespace Label {
            inline str SaveNotifyVariantLabel = "Save Notification Variant";
            inline str DisableSaveInCombatLabel = "Disable Saving in Combat";
            inline str SaveMessageLabel = "Notification for temp saves";
            inline str SaveAllowTimeLabel = "Book saving time";
        }
        namespace Tooltip {
            inline str SaveNotifyVariantLabel = "Type of notification to show that a temporary save is possible";
            inline str DisableSaveInCombatLabel = "Toggle saving mid combat";
            inline str SaveMessageLabel = "Message that shows when a temporary save is allowed. \nUse {} to have the time shown like this: \nTEMP SAVE ALLOWED FOR {} SECONDS";
            inline str SaveAllowTimeLabel = "Time that tempory saves are allowed";
        }
        //temp variables for settings
        inline int32_t save_notify_variant_temp;
        inline bool disable_save_in_combat_temp;
        inline str temp_save_message_temp;
        inline float temp_save_time_temp;
    }


}