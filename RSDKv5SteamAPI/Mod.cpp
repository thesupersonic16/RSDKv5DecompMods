#include "Mod.hpp"
#include "Steam.hpp"

bool32 EnabledDLC[DLC_COUNT];
void* ModHandle = nullptr;

// Hooks
HOOK(bool32, DEFAULT_CALL_TYPE, ShowExtensionOverlay, NULL, uint8 overlay)
{
    return Steam::OpenDLCStore(overlay);
}
HOOK(bool32, DEFAULT_CALL_TYPE, GetUsername, NULL, String* userName)
{
    const char* name = Steam::GetProfileName();
    if (name)
    {
        RSDK.SetString(userName, name);
        return true;
    }
    return originalGetUsername(userName);
}
HOOK(void, DEFAULT_CALL_TYPE, TryUnlockAchievement, NULL, AchievementID* id)
{
    Steam::UnlockAchievement(id->id);
}
HOOK(void, DEFAULT_CALL_TYPE, SetRichPresence, NULL, int32 id, String* message)
{
    char buffer[256];
    RSDK.GetCString(buffer, message);
    Steam::SetPresence(buffer);
}

HOOK(bool32, DEFAULT_CALL_TYPE, CheckDLC, NULL, uint8 id)
{
    if (id >= 0 && id <= 8)
        return EnabledDLC[id];
    return false;
}

void CheckDLCs()
{
    for (int i = 0; i < DLC_COUNT; ++i)
        EnabledDLC[i] = Steam::CheckDLCOwnership(i);
}

void OnUpdate(void* data)
{
    Steam::RunCallbacks();
}

void OnGameStartUp(void* data)
{
    CheckDLCs();
}

void InitModAPI()
{
    if (Steam::InitAPI())
    {
        originalShowExtensionOverlay = (ShowExtensionOverlay*)(API.ShowExtensionOverlay);
        originalGetUsername          = (GetUsername*)(API.GetUsername);
        originalTryUnlockAchievement = (TryUnlockAchievement*)(API.TryUnlockAchievement);
        originalSetRichPresence      = (SetRichPresence*)(API.SetRichPresence);
        originalCheckDLC             = (CheckDLC*)(API.CheckDLC);

        INSTALL_HOOK(ShowExtensionOverlay);
        INSTALL_HOOK(GetUsername);
        INSTALL_HOOK(TryUnlockAchievement);
        INSTALL_HOOK(SetRichPresence);
        INSTALL_HOOK(CheckDLC);

        memset(EnabledDLC, 0, sizeof(EnabledDLC));
        
        Mod.AddModCallback(MODCB_ONUPDATE, OnUpdate);
        Mod.AddModCallback(MODCB_ONGAMESTARTUP, OnGameStartUp);
    }
}

extern "C" DLLExport void UnloadMod()
{
    UNINSTALL_HOOK(ShowExtensionOverlay);
    UNINSTALL_HOOK(GetUsername);
    UNINSTALL_HOOK(TryUnlockAchievement);
    UNINSTALL_HOOK(SetRichPresence);
    UNINSTALL_HOOK(CheckDLC);
    Steam::ShutdownAPI();
}

extern "C" DLLExport bool32 LinkModLogic(EngineInfo* info, const char* id)
{
#if !RETRO_REV01
    LinkGameLogicDLL(info);
#else
    LinkGameLogicDLL(*info);
#endif
#ifdef GAME_IS_MANIA
    InitModAPI();
    return true;
#else
    return false;
#endif
}

