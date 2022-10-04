#define _CRT_SECURE_NO_WARNINGS
#include "Hook.hpp"
#include "framework.h"
#include <Game.hpp>
#include <steam_api.h>
#include <subhook.h>

#if RETRO_USE_MOD_LOADER
extern "C" {
DLLExport bool32 LinkModLogic(RSDK::EngineInfo* info, const char* id);
DLLExport void UnloadMod();
}
#endif

#define ADD_PUBLIC_FUNC(func) Mod.AddPublicFunction(#func, (void *)(func))

ManiaGlobalVariables* maniaGlobals
    = nullptr;

static bool EnableAchievements = false;

class SteamCallbacks
{
public:
    STEAM_CALLBACK(SteamCallbacks, OnUserStatsReceived, UserStatsReceived_t);
};

void SteamCallbacks::OnUserStatsReceived(UserStatsReceived_t* pCallback)
{
    EnableAchievements = true;
}

SteamCallbacks* callbacks;

Hook<bool32, int32> ShowExtensionOverlay;
bool32 ShowExtensionOverlay_r(int32 overlay)
{
    switch (overlay)
    {
    case 0:
        SteamFriends()->ActivateGameOverlayToWebPage("https://store.steampowered.com/app/845640/Sonic_Mania__Encore_DLC/");
        return true;
    default:
        errorMessage("Unknown Overlay ID!\n\nPlease report this issue!", "RSDKv5SteamAPI Error");
        return false;
    }
}

Hook<bool32, RSDK::String*> GetUsername;
bool32 GetUsername_r(RSDK::String* userName)
{
    const char* name = SteamFriends()->GetPersonaName();
    if (name)
    {
        RSDK::String::Copy(userName, name);
        return true;
    }
    return GetUsername.Original(userName);
}

Hook<void, RSDK::AchievementID*> TryUnlockAchievement;
void TryUnlockAchievement_r(RSDK::AchievementID* id)
{
    // Unlock the achievement and save
    if (EnableAchievements && RSDK::APITable->GetAchievementsEnabled())
    {
        SteamUserStats()->SetAchievement(id->id);
        SteamUserStats()->StoreStats();
    }
    TryUnlockAchievement.Original(id);
}

Hook<bool32, int32> CheckDLC;
bool32 CheckDLC_r(int32 dlc)
{
    // NOTE: Not sure why this does not work with the overlay disabled.
    /*
    AppId_t appId;
    bool available;
    char name[128];
    if (SteamApps()->BGetDLCDataByIndex(dlc, &appId, &available, name, 128))
        return SteamApps()->BIsDlcInstalled(appId);
    return false;
    */
    switch (dlc)
    {
    case 0:
        return SteamApps()->BIsDlcInstalled(845640);
    default:
        return CheckDLC.Original(dlc);
    }
}

Hook<void, int32, RSDK::String*> SetRichPresence;
void SetRichPresence_r(int32 id, RSDK::String* text)
{
    char cText[k_cchMaxRichPresenceValueLength];
    text->CStr(cText);
    SteamFriends()->SetRichPresence("status", cText);
    SetRichPresence.Original(id, text);
}

void OnUpdate(void* data)
{
    SteamAPI_RunCallbacks();
}

#define INSTALL_HOOK(hook) hook.Install(RSDK::APITable->hook, hook##_r);

bool32 LinkModLogic(RSDK::EngineInfo* info, const char* id)
{
#if !RETRO_REV01
    LinkGameLogicDLL(info);
#else
    LinkGameLogicDLL(*info);
#endif

    if (!SteamAPI_Init())
    {
        errorMessage("Failed to initialise the Steam API", "SteamAPI Error");
    }
    else
    {
        INSTALL_HOOK(ShowExtensionOverlay)
        INSTALL_HOOK(GetUsername)
        INSTALL_HOOK(TryUnlockAchievement)
        INSTALL_HOOK(SetRichPresence)
        INSTALL_HOOK(CheckDLC)

        if (!SteamUserStats()->RequestCurrentStats()) {
            errorMessage("Failed to request stats!\n\nAchievements will be disabled.", "SteamAPI Error");
        }

        callbacks = new SteamCallbacks();
        RSDK::Mod::AddModCallback(RSDK::MODCB_ONUPDATE, OnUpdate);
    }

    maniaGlobals = (ManiaGlobalVariables*)RSDK::Mod::GetGlobals();

    return true;
}

void UnloadMod()
{
    ShowExtensionOverlay.Remove();
    GetUsername.Remove();
    TryUnlockAchievement.Remove();
    SetRichPresence.Remove();
    CheckDLC.Remove();

    SteamAPI_Shutdown();
    delete callbacks;
}