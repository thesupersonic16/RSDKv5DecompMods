#include "PauseMenu.h"

#define PAUSEMENU_MENU_DURATION 60
#define PAUSEMENU_MENU_DELAY 24

ObjectPauseMenu* PauseMenu;

void PauseMenu_StopMusic()
{
    for (int i = 0; i < CHANNEL_COUNT; ++i)
    {
        bool32 channelActive = RSDK.ChannelActive(i);
        if (channelActive)
            RSDK.StopChannel(i);
    }
}

void PauseMenu_PauseMusic()
{
    for (int i = 0; i < CHANNEL_COUNT; ++i)
    {
        bool32 channelActive = RSDK.ChannelActive(i);

        PauseMenu->activeChannels[i] = channelActive;
        if (channelActive)
            RSDK.PauseChannel(i);
    }
}

void PauseMenu_ResumeMusic()
{
    for (int i = 0; i < CHANNEL_COUNT; ++i)
    {
        bool32 channelActive = RSDK.ChannelActive(i);

        if (PauseMenu->activeChannels[i])
            RSDK.ResumeChannel(i);
    }
}

void PauseMenu_Create(void* data)
{
    RSDK_THIS(PauseMenu);

    if (SceneInfo->inEditor)
        return;

    bool32 isSpecialStage = (SceneInfo->activeCategory == 4 ||
        SceneInfo->activeCategory == 5);

    self->active = ACTIVE_ALWAYS;
    self->visible = true;
    self->drawGroup = DRAWGROUP_COUNT - 1;
    self->state = PauseMenu_State_Pausing;
    self->stateDraw = StateMachine_None;
    self->fadeCompleted = false;
    self->buttonCount = SceneInfo->debugMode ? 4 : 3;
    self->buttonSelection = 0;
    self->buttonSelected = false;
    self->timer = 0;

    RSDK.SetSpriteAnimation(PauseMenu->aniPause, PAUSEMENU_BUTTON_COUNT, &self->animatorPlate, true, isSpecialStage ? 2 : !SceneInfo->debugMode);
    RSDK.SetSpriteAnimation(PauseMenu->aniPause, 0, &self->animatorSelection, true, 0);
    RSDK.SetSpriteAnimation(PauseMenu->aniPause, PAUSEMENU_BUTTON_COUNT + 1, &self->animatorHighlight, true, 0);


    RSDK.SetEngineState(ENGINESTATE_FROZEN);
    PauseMenu_PauseMusic();
    RSDK.PlaySfx(PauseMenu->sfxWoosh, 0, 1);

    for (int i = 0; i < PAUSEMENU_BUTTON_COUNT; ++i)
        self->buttonActions[i] = NULL;
 
    // Continue
    self->buttonActions[0] = PauseMenu_Action_Continue;
    self->buttonEnables[0] = true;

    // Restart
    self->buttonActions[1] = PauseMenu_Action_Restart;
    self->buttonEnables[1] = true;

    // Exit
    self->buttonActions[2] = PauseMenu_Action_Exit;
    self->buttonEnables[2] = true;

    // Level Select
    self->buttonActions[3] = PauseMenu_Action_LevelSelect;
    self->buttonEnables[3] = true;

    // Disable on special stages
    if (isSpecialStage)
    {
        self->buttonEnables[1] = false;
        self->buttonEnables[2] = false;
        self->buttonEnables[3] = false;
        self->buttonCount = 1;
    }
}

void PauseMenu_Update(void)
{
    RSDK_THIS(PauseMenu);

    if (SceneInfo->inEditor)
        return;

    StateMachine_Run(self->state);
    ++self->timer;
}

void PauseMenu_Draw(void)
{
    RSDK_THIS(PauseMenu);

    // Copy palette
    if (SceneInfo->activeCategory == 5)
        RSDK.CopyPalette(7, 16 * 8, 0, 0, 16 * 2);


    if (self->buttonSelected && self->timer < PAUSEMENU_MENU_DELAY)
        self->animatorSelection.frameID = (self->timer / 2) % 2;
    else
        self->animatorSelection.frameID = 0;

    Vector2 pos;
    pos.x = TO_FIXED(ScreenInfo->size.x / 2);
    pos.y = TO_FIXED(ScreenInfo->size.y / 2);
    RSDK.DrawSprite(&self->animatorPlate, &pos, true);
    RSDK.DrawSprite(&self->animatorHighlight, &pos, true);
    RSDK.DrawSprite(&self->animatorSelection, &pos, true);

    StateMachine_Run(self->stateDraw);
}

void PauseMenu_StageLoad(void)
{
    PauseMenu->sfxBleep = RSDK.GetSfx("Global/MenuBleep.wav");
    PauseMenu->sfxAccept = RSDK.GetSfx("Global/MenuAccept.wav");
    PauseMenu->sfxWoosh = RSDK.GetSfx("Global/MenuWoosh.wav");

    PauseMenu->aniPause = RSDK.LoadSpriteAnimation("3K_Global/Pause.bin", SCOPE_STAGE);
}

void PauseMenu_StaticUpdate(void)
{
}

void PauseMenu_StaticLoad(ObjectPauseMenu* obj)
{
}

void PauseMenu_State_Pausing(void)
{
    RSDK_THIS(PauseMenu);

//    if (self->timer == PAUSEMENU_MENU_DELAY)
    self->state = PauseMenu_State_Paused;
}

void PauseMenu_State_Resuming(void)
{
    RSDK_THIS(PauseMenu);

    if (self->timer >= PAUSEMENU_MENU_DELAY)
    {
        PauseMenu_ResumeMusic();
        RSDK.SetEngineState(ENGINESTATE_REGULAR);
        RSDK.ResetEntity(self, 0, NULL);
    }
}

void PauseMenu_State_Restarting(void)
{
    RSDK_THIS(PauseMenu);

    if (self->fadeCompleted)
    {
        RSDK.SetEngineState(ENGINESTATE_LOAD);
        RSDK.ResetEntity(self, 0, NULL);
        PauseMenu_StopMusic();
    }
}

void PauseMenu_State_ReturningToTitle(void)
{
    RSDK_THIS(PauseMenu);

    if (self->fadeCompleted)
    {
        RSDK.SetScene("Presentation & Menus", "Title Screen");
        RSDK.SetEngineState(ENGINESTATE_LOAD);
        RSDK.ResetEntity(self, 0, NULL);
        PauseMenu_StopMusic();
    }
}

void PauseMenu_State_EnteringLevelSelect(void)
{
    RSDK_THIS(PauseMenu);

    if (self->fadeCompleted)
    {
        RSDK.SetScene("Presentation & Menus", "Level Select");
        RSDK.SetEngineState(ENGINESTATE_LOAD);
        RSDK.ResetEntity(self, 0, NULL);
        PauseMenu_StopMusic();
    }
}

void PauseMenu_State_Paused(void)
{
    RSDK_THIS(PauseMenu);

    RSDKControllerState* controller = &ControllerInfo[0];
    bool32 selectionCanged = false;

    if (controller->keyA.press || controller->keyStart.press)
    {
        self->buttonSelected = true;
        RSDK.PlaySfx(PauseMenu->sfxAccept, 0, 1);
        if (self->buttonActions[self->buttonSelection])
            self->buttonActions[self->buttonSelection]();
    }
    else if (controller->keyUp.press)
    {
        RSDK.PlaySfx(PauseMenu->sfxBleep, 0, 1);
        do
        {
            --self->buttonSelection;

            if (self->buttonSelection < 0)
                self->buttonSelection = self->buttonCount - 1;

            selectionCanged = true;
        } while (!self->buttonEnables[self->buttonSelection]);
    }
    else if (controller->keyDown.press)
    {
        RSDK.PlaySfx(PauseMenu->sfxBleep, 0, 1);
        do
        {
            ++self->buttonSelection;

            if (self->buttonSelection >= self->buttonCount)
                self->buttonSelection = 0;

            selectionCanged = true;
        } while (!self->buttonEnables[self->buttonSelection]);
    }

    if (selectionCanged)
    {
        RSDK.SetSpriteAnimation(PauseMenu->aniPause, self->buttonSelection, &self->animatorSelection, false, 0);
        RSDK.SetSpriteAnimation(PauseMenu->aniPause, PAUSEMENU_BUTTON_COUNT + 1, &self->animatorHighlight, true, self->buttonSelection);
    }
}

void PauseMenu_Draw_FadeOut(void)
{
    RSDK_THIS(PauseMenu);
    float progress = ((self->timer - PAUSEMENU_MENU_DELAY) / (float)PAUSEMENU_MENU_DURATION) * 320.0f;
    int32 alpha = (int32)progress;
    RSDK.FillScreen(0x000000, alpha, alpha - 32, alpha - 64);
    if (alpha >= 320)
        self->fadeCompleted = true;
}

void PauseMenu_Action_Continue(void)
{
    RSDK_THIS(PauseMenu);
    
    self->timer = 0;
    self->state = PauseMenu_State_Resuming;
}

void PauseMenu_Action_Restart(void)
{
    RSDK_THIS(PauseMenu);

    self->timer = 0;
    self->state = PauseMenu_State_Restarting;
    self->stateDraw = PauseMenu_Draw_FadeOut;
}
void PauseMenu_Action_Exit(void)
{
    RSDK_THIS(PauseMenu);

    self->timer = 0;
    self->state = PauseMenu_State_ReturningToTitle;
    self->stateDraw = PauseMenu_Draw_FadeOut;
}

void PauseMenu_Action_LevelSelect(void)
{
    RSDK_THIS(PauseMenu);

    self->timer = 0;
    self->state = PauseMenu_State_EnteringLevelSelect;
    self->stateDraw = PauseMenu_Draw_FadeOut;
}
