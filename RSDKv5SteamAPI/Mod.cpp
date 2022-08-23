#define _CRT_SECURE_NO_WARNINGS
#include <Game.hpp>
#include <steam_api.h>
#include "Hook.hpp"

#define ADD_PUBLIC_FUNC(func) Mod.AddPublicFunction(#func, (void *)(func))

ManiaGlobalVariables* maniaGlobals = nullptr;

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

Func<bool32, int32>* ShowExtensionOverlay;
bool32 ShowExtensionOverlay_r(int32 overlay)
{
    switch (overlay)
    {
    case 0:
        SteamFriends()->ActivateGameOverlayToWebPage("https://store.steampowered.com/app/845640/Sonic_Mania__Encore_DLC/");
        return true;
    default:
        MessageBoxA(NULL, "Unknown Overlay ID!\n\nPlease report this issue!", "RSDKv5SteamAPI Error", MB_ICONERROR);
        return false;
    }
}

Func<bool32, RSDK::String*>* GetUsername;
bool32 GetUsername_r(RSDK::String* userName)
{
    const char* name = SteamFriends()->GetPersonaName();
    if (name)
    {
        RSDK::String::Copy(userName, name);
        return true;
    }
    return GetUsername->Original(userName);
}

Func<bool32, RSDK::AchievementID*>* TryUnlockAchievement;
bool32 TryUnlockAchievement_r(RSDK::AchievementID* id)
{
    // Unlock the achievement and save
    if (EnableAchievements && RSDK::APITable->GetAchievementsEnabled())
    {
        SteamUserStats()->SetAchievement(id->id);
        SteamUserStats()->StoreStats();
    }
    return TryUnlockAchievement->Original(id);
}

Func<bool32, bool32>* CheckDLC;
bool32 CheckDLC_r(bool32 dlc)
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
        return CheckDLC->Original(dlc);
    }
}

Func<bool32, int32, RSDK::String*>* SetRichPresence;
bool32 SetRichPresence_r(int32 id, RSDK::String* text)
{
    char cText[k_cchMaxRichPresenceValueLength];
    text->CStr(cText);
    SteamFriends()->SetRichPresence("status", cText);
    return SetRichPresence->Original(id, text);
}

void OnUpdate(void* data)
{
    SteamAPI_RunCallbacks();
}

extern "C" __declspec(dllexport) bool32 LinkModLogic(RSDK::EngineInfo * info, const char* id)
{
#if !RETRO_REV01
    LinkGameLogicDLL(info);
#else
    LinkGameLogicDLL(*info);
#endif

    if (!SteamAPI_Init())
    {
        MessageBoxA(NULL, "Failed to initialise the Steam API", "SteamAPI Error", MB_ICONERROR);
    }
    else
    {
        ShowExtensionOverlay = new Func<bool32, int32>(RSDK::APITable->ShowExtensionOverlay);
        ShowExtensionOverlay->Hook(ShowExtensionOverlay_r);
        GetUsername = new Func<bool32, RSDK::String*>(RSDK::APITable->GetUsername);
        GetUsername->Hook(GetUsername_r);
        TryUnlockAchievement = new Func<bool32, RSDK::AchievementID*>(RSDK::APITable->TryUnlockAchievement);
        TryUnlockAchievement->Hook(TryUnlockAchievement_r);
        SetRichPresence = new Func<bool32, int32, RSDK::String*>(RSDK::APITable->SetRichPresence);
        SetRichPresence->Hook(SetRichPresence_r);
        CheckDLC = new Func<bool32, bool32>(RSDK::APITable->CheckDLC);
        CheckDLC->Hook(CheckDLC_r);

        if (!SteamUserStats()->RequestCurrentStats())
        {
            MessageBoxA(NULL, "Failed to request stats!\n\nAchievements will be disabled.", "SteamAPI Error", MB_ICONERROR);
        }
        
        callbacks = new SteamCallbacks();
        RSDK::Mod::AddModCallback(RSDK::MODCB_ONUPDATE, OnUpdate);
    }

    maniaGlobals = (ManiaGlobalVariables*)RSDK::Mod::GetGlobals();

    return true;
}

#define UNLOAD_HOOK(hook) \
    if (hook) \
    { \
        hook->Unhook(); \
        delete hook; \
    }

extern "C" __declspec(dllexport) void UnloadMod()
{
    UNLOAD_HOOK(ShowExtensionOverlay);
    UNLOAD_HOOK(GetUsername);
    UNLOAD_HOOK(TryUnlockAchievement);
    UNLOAD_HOOK(SetRichPresence);
    UNLOAD_HOOK(CheckDLC);

    SteamAPI_Shutdown();
    delete callbacks;
}