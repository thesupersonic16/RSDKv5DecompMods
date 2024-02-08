#include "Objects/PauseMenu.h"

#define Player_Input_Gamepad ((Type_StateMachine)(0x1401E42A0))
#define S3K_SS_Player_Input_Gamepad ((Type_StateMachine)(0x1402F03A0))

void OnStartUp(void* data)
{

}

bool32 Player_Input_Gamepad_Hook(bool32 skippedState, void* data)
{
    if (SceneInfo->state == ENGINESTATE_REGULAR
        && !PauseMenu->disableEvents
        && ((Entity*)RSDK.GetEntity(SLOT_PLAYER1))->classID != 0
        && ((Entity*)RSDK.GetEntity(SLOT_ACTCLEAR))->classID == 0
        //&& ((Entity*)RSDK.GetEntity(SLOT_CUTSCENESEQ))->classID == 0
        && ((Entity*)RSDK.GetEntity(SLOT_PAUSEMENU))->classID == 0)
    {
        if (ControllerInfo[0].keyStart.press)
            RSDK.ResetEntitySlot(16, PauseMenu->classID, NULL);
        //printf("Can spawn pause menu\n");
    }
    return false;
}

bool32 S3K_SS_Player_Input_Gamepad_Hook(bool32 skippedState, void* data)
{
    return Player_Input_Gamepad_Hook(skippedState, data);
}

void InitModAPI(void)
{
    Mod.RegisterStateHook(Player_Input_Gamepad, Player_Input_Gamepad_Hook, false);
    Mod.RegisterStateHook(S3K_SS_Player_Input_Gamepad, S3K_SS_Player_Input_Gamepad_Hook, false);
    MOD_REGISTER_OBJECT(PauseMenu, "PauseMenu", PauseMenu_Update, NULL, PauseMenu_StaticUpdate, PauseMenu_Draw, PauseMenu_Create, PauseMenu_StageLoad, NULL, NULL, NULL);
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