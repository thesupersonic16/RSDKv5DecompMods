#include "OptionsMenu.h"

#define S3K_SaveSlot_Create ((Type_StateMachine)(0x1403065D0))
#define ObjS3K_SaveSlot (0x143FB9FB8)

ObjectOptionsMenu* OptionsMenu;

void OptionsMenu_Create(void* data)
{
    RSDK_THIS(OptionsMenu);

    if (SceneInfo->inEditor)
        return;

    self->active = ACTIVE_ALWAYS;
    self->visible = true;
    self->drawGroup = 14;
    self->state = OptionsMenu_State_Menu;
}

void OptionsMenu_Update(void)
{
    RSDK_THIS(OptionsMenu);

    if (SceneInfo->inEditor)
        return;

    StateMachine_Run(self->state);
}

void OptionsMenu_Draw(void)
{
    RSDK_THIS(OptionsMenu);
    GlobalS3KVariables* S3kGlobals = (GlobalS3KVariables*)Mod.GetGlobals();

    Vector2 pos;
    pos.x = 0;
    pos.y = 0;

    // Icons
    RSDK.SetSpriteAnimation(OptionsMenu->ani, 0, &self->animator, true, S3kGlobals->playMode != 0);
    RSDK.DrawSprite(&self->animator, &pos, true);
    RSDK.SetSpriteAnimation(OptionsMenu->ani, 0, &self->animator, true, (S3kGlobals->unknown4C34D8 == 0) + 2);
    RSDK.DrawSprite(&self->animator, &pos, true);

    // Options
    RSDK.SetSpriteAnimation(OptionsMenu->ani, 2, &self->animator, true, S3kGlobals->playMode != 0);
    RSDK.DrawSprite(&self->animator, &pos, true);
    RSDK.SetSpriteAnimation(OptionsMenu->ani, 2, &self->animator, true, (S3kGlobals->unknown4C34D8 == 0) + 2);
    RSDK.DrawSprite(&self->animator, &pos, true);

    // Modes
    RSDK.SetSpriteAnimation(OptionsMenu->ani, 1, &self->animator, true, 0);
    RSDK.DrawSprite(&self->animator, &pos, true);
    RSDK.SetSpriteAnimation(OptionsMenu->ani, 1, &self->animator, true, 1);
    RSDK.DrawSprite(&self->animator, &pos, true);
}

void OptionsMenu_StageLoad(void)
{
    OptionsMenu->sfxBleep = RSDK.GetSfx("Stage/Button.wav");
    OptionsMenu->ani = RSDK.LoadSpriteAnimation("3K_Menu/OptionsMenu.bin", SCOPE_STAGE);
}

void OptionsMenu_State_Menu(void)
{
    RSDK_THIS(OptionsMenu);
    RSDKControllerState* controller = &ControllerInfo[0];
    GlobalS3KVariables* S3kGlobals = (GlobalS3KVariables*)Mod.GetGlobals();
    bool32 updateSaveSlots = false;

    if (controller->keyC.press)
    {
        RSDK.PlaySfx(OptionsMenu->sfxBleep, 0, 0);
        S3kGlobals->playMode = !S3kGlobals->playMode;

        if (S3kGlobals->playMode & 1)
        {
            S3kGlobals->disableLives = true;
            S3kGlobals->useCoins = true;
        }
        else
        {
            S3kGlobals->disableLives = false;
            S3kGlobals->useCoins = false;
            S3kGlobals->unknown4C34D8 = false;
        }
        updateSaveSlots = true;
    }
    if (controller->keyZ.press)
    {
        RSDK.PlaySfx(OptionsMenu->sfxBleep, 0, 0);
        S3kGlobals->unknown4C34D8 ^= 1;

        if (S3kGlobals->unknown4C34D8)
            S3kGlobals->playMode = 3;
        else
            S3kGlobals->playMode = 1;

        S3kGlobals->disableLives = true;
        S3kGlobals->useCoins = true;

        updateSaveSlots = true;
    }

    if (updateSaveSlots)
    {
        Entity* saveSlot = NULL;                    

        while (RSDK.GetAllEntities((*(Object**)ObjS3K_SaveSlot)->classID, (void**)&saveSlot))
        {
            StateMachine_Run2(S3K_SaveSlot_Create, saveSlot);
        }
    }

}