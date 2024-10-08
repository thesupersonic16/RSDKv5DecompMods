#include "S3KExtensions.hpp"

S3KGlobalVariables *globals = nullptr;
S3KExtensionsGlobalVariables *modGlobals = nullptr;
S3KExtensionsConfig config;

using namespace RSDK;
using namespace GameLogic;
using namespace Helpers;

void LinkGameLogic(RSDK::EngineInfo *info) {
    // Globals
    globals = (S3KGlobalVariables *)Mod::GetGlobals();
    modTable->RegisterGlobals("Data/Game/S3KExtensionsGlobals.bin", (void**)&modGlobals, sizeof(S3KExtensionsGlobalVariables), nullptr);

    // Mod Config
    config.optionsMenuEnable = Mod::Settings::GetBool("", "OptionsMenu:Enable", true);
    config.pauseMenuEnable = Mod::Settings::GetBool("", "PauseMenu:Enable", true);
    config.missionHandlerEnable = Mod::Settings::GetBool("", "MissionHandler:Enable", true);
    config.debugMenuEnable = Mod::Settings::GetBool("", "DebugMenu:Enable", true);
    Mod::Settings::SaveSettings();

    // Object States
    S3K_SaveMenu::State_Init.Set(ToMember<S3K_SaveMenu>(Mod::GetPublicFunction(nullptr, "S3K_SaveMenu::State_Init")));
    Player::Input_Gamepad.Set(ToMember<Player>(Mod::GetPublicFunction(nullptr, "Player::Input_Gamepad")));
    S3K_SS_Player::Input_Gamepad.Set(ToMember<S3K_SS_Player>(Mod::GetPublicFunction(nullptr, "S3K_SS_Player::Input_Gamepad")));
    SignPost2::State_Falling.Set(ToMember<SignPost2>(Mod::GetPublicFunction(nullptr, "SignPost::State_Falling")));
    HUD::DrawNumbersBase10 = (void (*)(HUD *, Animator *, Vector2 *, int32, int32))Mod::GetPublicFunction(nullptr, "HUD::DrawNumbersBase10");
    HUD::DrawNumbersBase16 = (void (*)(HUD *, Vector2 *, int32))Mod::GetPublicFunction(nullptr, "HUD::DrawNumbersBase16");

    // State Hooks
    if (config.optionsMenuEnable)
        modTable->RegisterStateHook(ToGeneric<S3K_SaveMenu>(S3K_SaveMenu::State_Init.state), S3K_SaveMenu::State_Init_Hook, true);

    if (config.pauseMenuEnable) {
        modTable->RegisterStateHook(ToGeneric<Player>(Player::Input_Gamepad.state), Player::Input_Gamepad_Hook, true);
        modTable->RegisterStateHook(ToGeneric<S3K_SS_Player>(S3K_SS_Player::Input_Gamepad.state), S3K_SS_Player::Input_Gamepad_Hook, true);
    }

    if (config.missionHandlerEnable) {
        // Callbacks
        EngineNotifyCallback = RSDKTable->NotifyCallback;
        RSDKTable->NotifyCallback = OnNotifyCallback;
        Mod::AddModCallback(MODCB_ONSTAGELOAD, OnStageLoad);

        // I need to do something about this
        FILE *file;
        fopen_s(&file, "mission_rfl_param.rfl", "rb");
        if (file) {
            fread(modGlobals->missionRFL, 1, 0x4000, file);
            fclose(file);
            modGlobals->misionInfo = Origins::MissionInfo::ParseRFL(modGlobals->missionRFL);
        }
    }
}