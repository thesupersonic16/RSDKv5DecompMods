#include "S3KExtensions.hpp"

S3KGlobalVariables *globals = nullptr;
S3KExtensionsConfig config;

using namespace RSDK;
using namespace GameLogic;
using namespace Helpers;

void LinkGameLogic(RSDK::EngineInfo *info) {
    // ---------------------
    // Link Global Variables
    // ---------------------

    globals = (S3KGlobalVariables *)Mod::GetGlobals();

    // -------------------------
    // Mod Config Initialization
    // -------------------------

    config.optionsMenuEnable = Mod::Settings::GetBool("", "OptionsMenu:Enable", true);
    config.pauseMenuEnable = Mod::Settings::GetBool("", "PauseMenu:Enable", true);
    Mod::Settings::SaveSettings();

    // -----------------
    // Get Object States
    // -----------------

    S3K_SaveMenu::State_Init.Set(ToMember<S3K_SaveMenu>(Mod::GetPublicFunction(nullptr, "S3K_SaveMenu::State_Init")));
    Player::Input_Gamepad.Set(ToMember<Player>(Mod::GetPublicFunction(nullptr, "Player::Input_Gamepad")));
    S3K_SS_Player::Input_Gamepad.Set(ToMember<S3K_SS_Player>(Mod::GetPublicFunction(nullptr, "S3K_SS_Player::Input_Gamepad")));

    // --------------------
    // Register State Hooks
    // --------------------

    if (config.optionsMenuEnable)
        modTable->RegisterStateHook(ToGeneric<S3K_SaveMenu>(S3K_SaveMenu::State_Init.state), S3K_SaveMenu::State_Init_Hook, true);

    if (config.pauseMenuEnable) {
        modTable->RegisterStateHook(ToGeneric<Player>(Player::Input_Gamepad.state), Player::Input_Gamepad_Hook, true);
        modTable->RegisterStateHook(ToGeneric<S3K_SS_Player>(S3K_SS_Player::Input_Gamepad.state), S3K_SS_Player::Input_Gamepad_Hook, true);
    }

}