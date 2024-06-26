#include "Objects/OptionsMenu.h"
#include "framework.h"

StateMachine(S3K_SaveMenu_State_Init);

int32 UltrafixDetected = -1;

void OnStartUp(void* data)
{

}

void nullfunc(void) {};

bool32 OptionsMenu_State_CreateHook(bool32 skippedState, Entity* entity)
{
    if (UltrafixDetected == -1)
        UltrafixDetected = (LoadLibraryA("Ultrafix3kFixes.dll") != 0);

    // Load UF.act if Ultrafix is detected
    if (UltrafixDetected)
        RSDK.LoadPalette(0, "UF.act", 0xFEEC);

    if (((Entity*)RSDK.GetEntity(SLOT_PAUSEMENU))->classID == 0)
    {
        RSDK.ResetEntitySlot(SLOT_PAUSEMENU, OptionsMenu->classID, NULL);
    }
    return false;
}

void InitModAPI(void)
{
    S3K_SaveSlot_Create = Mod.GetPublicFunction(NULL, "S3K_SaveSlot::Create");
    S3K_SaveMenu_State_Init = Mod.GetPublicFunction(NULL, "S3K_SaveMenu::State_Init");

    Mod.RegisterStateHook(S3K_SaveMenu_State_Init, OptionsMenu_State_CreateHook, 0);
    MOD_REGISTER_OBJECT(OptionsMenu, "OptionsMenu", OptionsMenu_Update, OptionsMenu_LateUpdate, nullfunc, OptionsMenu_Draw, OptionsMenu_Create, OptionsMenu_StageLoad, nullfunc, nullfunc, nullfunc);
}

DLLExport bool32 LinkModLogic(EngineInfo* info, const char* id)
{
#if !RETRO_REV01
    LinkGameLogicDLL(info);
#else
    LinkGameLogicDLL(*info);
#endif
#ifdef GAME_IS_S3
    InitModAPI();

    return true;
#else
    return false;
#endif
}

DLLExport void UnloadMod()
{
}