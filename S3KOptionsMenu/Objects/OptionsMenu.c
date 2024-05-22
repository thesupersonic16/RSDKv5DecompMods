#include "OptionsMenu.h"

ObjectOptionsMenu* OptionsMenu;
StateMachine(S3K_SaveSlot_Create);

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

    bool32 isSmall = ScreenInfo->size.x < 330;

    Vector2 posGameIcon;
    Vector2 posGameHeader;
    Vector2 posGameOption;

    Vector2 posMirrorIcon;
    Vector2 posMirrorHeader;
    Vector2 posMirrorOption;

    if (isSmall)
    {
        posGameIcon.x = TO_FIXED(8);
        posGameIcon.y = TO_FIXED(ScreenInfo->size.y - 40);
        posGameHeader.x = TO_FIXED(8);
        posGameHeader.y = TO_FIXED(ScreenInfo->size.y - 23);
        posGameOption.x = TO_FIXED(8);
        posGameOption.y = TO_FIXED(ScreenInfo->size.y - 15);

        posMirrorIcon.x = TO_FIXED(ScreenInfo->size.x - 24);
        posMirrorIcon.y = TO_FIXED(ScreenInfo->size.y - 40);
        posMirrorHeader.x = TO_FIXED(ScreenInfo->size.x - 96);
        posMirrorHeader.y = TO_FIXED(ScreenInfo->size.y - 23);
        posMirrorOption.x = TO_FIXED(ScreenInfo->size.x - 8);
        posMirrorOption.y = TO_FIXED(ScreenInfo->size.y - 15);
    }
    else
    {
        posGameIcon.x = TO_FIXED(8);
        posGameIcon.y = TO_FIXED(ScreenInfo->size.y - 24);
        posGameHeader.x = TO_FIXED(25);
        posGameHeader.y = TO_FIXED(ScreenInfo->size.y - 23);
        posGameOption.x = TO_FIXED(25);
        posGameOption.y = TO_FIXED(ScreenInfo->size.y - 15);

        posMirrorIcon.x = TO_FIXED(ScreenInfo->size.x - 24);
        posMirrorIcon.y = TO_FIXED(ScreenInfo->size.y - 24);
        posMirrorHeader.x = TO_FIXED(ScreenInfo->size.x - 113);
        posMirrorHeader.y = TO_FIXED(ScreenInfo->size.y - 23);
        posMirrorOption.x = TO_FIXED(ScreenInfo->size.x - 25);
        posMirrorOption.y = TO_FIXED(ScreenInfo->size.y - 15);
    }

    // Game Mode Icon
    RSDK.SetSpriteAnimation(OptionsMenu->ani, 0, &self->animator, true, S3kGlobals->playMode != 0);
    RSDK.DrawSprite(&self->animator, &posGameIcon, true);
    // Game Mode Header
    RSDK.SetSpriteAnimation(OptionsMenu->ani, 1, &self->animator, true, 0);
    RSDK.DrawSprite(&self->animator, &posGameHeader, true);
    // Game Mode Option
    RSDK.SetSpriteAnimation(OptionsMenu->ani, 2, &self->animator, true, S3kGlobals->playMode != 0);
    RSDK.DrawSprite(&self->animator, &posGameOption, true);

    // Mirror Mode icon
    RSDK.SetSpriteAnimation(OptionsMenu->ani, 0, &self->animator, true, (S3kGlobals->unknown4C34D8 == 0) + 2);
    RSDK.DrawSprite(&self->animator, &posMirrorIcon, true);
    // Mirror Mode Header
    RSDK.SetSpriteAnimation(OptionsMenu->ani, 1, &self->animator, true, 1);
    RSDK.DrawSprite(&self->animator, &posMirrorHeader, true);
    // Mirror Mode Option
    RSDK.SetSpriteAnimation(OptionsMenu->ani, 2, &self->animator, true, (S3kGlobals->unknown4C34D8 == 0) + 2);
    RSDK.DrawSprite(&self->animator, &posMirrorOption, true);

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

        while (RSDK.GetAllEntities(RSDK.FindObject("S3K_SaveSlot"), (void**)&saveSlot))
        {
            StateMachine_Run2(S3K_SaveSlot_Create, saveSlot);
        }
    }

}