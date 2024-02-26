#define _CRT_SECURE_NO_WARNINGS
#include "Steam.hpp"
// Workaround
#undef false
#undef true
#include <string>
#pragma warning(push)
#pragma warning(disable : 4819)
#include <steam_api.h>
#pragma warning(pop)


class SteamCallbacks
{
public:
    STEAM_CALLBACK(SteamCallbacks, OnUserStatsReceived, UserStatsReceived_t);
    STEAM_CALLBACK(SteamCallbacks, GameOverlayActivated, GameOverlayActivated_t);
};


SteamCallbacks* SteamCallbacksInstance;
bool32 SteamUserStatsReceived = false;

void SteamCallbacks::OnUserStatsReceived(UserStatsReceived_t* pCallback)
{
    SteamUserStatsReceived = pCallback->m_eResult == k_EResultOK;
    RSDK.PrintLog(PRINT_NORMAL, "Logged into Steam account %lld", pCallback->m_steamIDUser.GetAccountID());
}

void SteamCallbacks::GameOverlayActivated(GameOverlayActivated_t* pCallback)
{
    // Check for DLC when the overlay closes
    if (!pCallback->m_bActive)
        CheckDLCs();
}

#ifdef STEAM_API_NODLL
HMODULE steamAPIHandle = nullptr;

S_API bool S_CALLTYPE SteamAPI_Init()
{
    if (steamAPIHandle) {
        void* address = GetProcAddress(steamAPIHandle, "SteamAPI_Init");
        return ((bool(S_CALLTYPE*)())(address))();
    }
    return false;
}

S_API void S_CALLTYPE SteamAPI_Shutdown()
{
    if (steamAPIHandle) {
        void* address = GetProcAddress(steamAPIHandle, "SteamAPI_Shutdown");
        return ((void(S_CALLTYPE*)())(address))();
    }
}

S_API void S_CALLTYPE SteamAPI_RunCallbacks()
{
    if (steamAPIHandle) {
        void* address = GetProcAddress(steamAPIHandle, "SteamAPI_RunCallbacks");
        return ((void(S_CALLTYPE*)())(address))();
    }
}

S_API bool S_CALLTYPE SteamAPI_IsSteamRunning()
{
    if (steamAPIHandle) {
        void* address = GetProcAddress(steamAPIHandle, "SteamAPI_IsSteamRunning");
        return ((bool(S_CALLTYPE*)())(address))();
    }
    return false;
}

S_API HSteamPipe SteamAPI_GetHSteamPipe()
{
    if (steamAPIHandle) {
        void* address = GetProcAddress(steamAPIHandle, "SteamAPI_GetHSteamPipe");
        return ((HSteamPipe(S_CALLTYPE*)())(address))();
    }
    return 0;
}

S_API HSteamUser SteamAPI_GetHSteamUser()
{
    if (steamAPIHandle) {
        void* address = GetProcAddress(steamAPIHandle, "SteamAPI_GetHSteamUser");
        return ((HSteamUser(S_CALLTYPE*)())(address))();
    }
    return 0;
}

S_API void* S_CALLTYPE SteamInternal_ContextInit(void* pContextInitData)
{
    if (steamAPIHandle) {
        void* address = GetProcAddress(steamAPIHandle, "SteamInternal_ContextInit");
        return ((void* (S_CALLTYPE*)(void* pContextInitData))(address))(pContextInitData);
    }
    return nullptr;
}

S_API void* S_CALLTYPE SteamInternal_CreateInterface(const char* ver)
{
    if (steamAPIHandle) {
        void* address = GetProcAddress(steamAPIHandle, "SteamInternal_CreateInterface");
        return ((void* (S_CALLTYPE*)(const char* ver))(address))(ver);
    }
    return nullptr;
}

S_API void S_CALLTYPE SteamAPI_RegisterCallback(class CCallbackBase* pCallback, int iCallback)
{
    if (steamAPIHandle) {
        void* address = GetProcAddress(steamAPIHandle, "SteamAPI_RegisterCallback");
        ((void (S_CALLTYPE*)(class CCallbackBase* pCallback, int iCallback))(address))(pCallback, iCallback);
    }
}

S_API void S_CALLTYPE SteamAPI_UnregisterCallback(class CCallbackBase* pCallback)
{
    if (steamAPIHandle) {
        void* address = GetProcAddress(steamAPIHandle, "SteamAPI_UnregisterCallback");
        ((void (S_CALLTYPE*)(class CCallbackBase* pCallback))(address))(pCallback);
    }
}

#endif

bool Steam::OpenDLCStore(uint8 id)
{
    switch (id)
    {
    case 0:
        SteamFriends()->ActivateGameOverlayToWebPage("https://store.steampowered.com/app/845640/Sonic_Mania__Encore_DLC/");
        return true;
    default:
        return false;
    }
}

const char* Steam::GetProfileName()
{
    return SteamFriends()->GetPersonaName();
}

void Steam::UnlockAchievement(const char* id)
{
    if (SteamUserStatsReceived && API.GetAchievementsEnabled())
    {
        SteamUserStats()->SetAchievement(id);
        SteamUserStats()->StoreStats();
    }
}

void Steam::SetPresence(const char* text)
{
    SteamFriends()->SetRichPresence("status", text);
}

bool Steam::CheckDLCOwnership(uint8 id)
{
    switch (id)
    {
    case 0:
        return SteamApps()->BIsDlcInstalled(845640);
    default:
        return false;
    }

}

bool Steam::InitAPI()
{
#ifdef STEAM_API_NODLL
    char oldWorkingDirectory[MAX_PATH];
    GetCurrentDirectoryA(MAX_PATH, oldWorkingDirectory);
    char mainModulePath[MAX_PATH];
    if (GetModuleFileNameA((HMODULE)ModHandle, mainModulePath, MAX_PATH) == 0)
    {
        DisplayError("Failed to get module file name.");
        return false;
    }

    std::string modulePath = mainModulePath;
    std::string::size_type slashPos = modulePath.find_last_of("\\/");
    std::string directory = modulePath.substr(0, slashPos);
    SetCurrentDirectoryA(directory.c_str());
    if (!(steamAPIHandle = LoadLibraryA(STEAM_DLL_NAME)))
    {
        DisplayError("Failed to load SteamAPI.");
        steamAPIHandle = nullptr;
        SetCurrentDirectoryA(oldWorkingDirectory);
        return false;
    }
#endif

    if (!SteamAPI_Init())
    {
        DisplayError("Failed to initialise the Steam API.");
#ifdef STEAM_API_NODLL
        SetCurrentDirectoryA(oldWorkingDirectory);
#endif
        return false;
    }
    else
    {
        SteamCallbacksInstance = new SteamCallbacks();
        if (!SteamUserStats()->RequestCurrentStats())
            DisplayError("Steam not logged in.");
    }
#ifdef STEAM_API_NODLL
    SetCurrentDirectoryA(oldWorkingDirectory);
#endif
    return true;
}

void Steam::ShutdownAPI()
{
    SteamAPI_Shutdown();
}

void Steam::RunCallbacks()
{
    SteamAPI_RunCallbacks();
}
