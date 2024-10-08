#include "../S3KExtensions.hpp"

using namespace RSDK;

namespace GameLogic {

// -------------------
// Object Registration
// -------------------

MOD_REGISTER_OBJECT(PauseMenu);

// ----------------------
// Standard Entity Events
// ----------------------

void PauseMenu::Create(void *data) {
    if (sceneInfo->inEditor)
        return;

    this->inSpecialStage = (sceneInfo->activeCategory == 4 || sceneInfo->activeCategory == 5)
        && globals->playMode != BOOT_PLAYMODE_MISSION;

    this->active = ACTIVE_ALWAYS;
    this->visible = true;
    this->drawGroup = DRAWGROUP_COUNT - 1;
    this->fadeCompleted = false;
    this->buttonCount = sceneInfo->debugMode ? 4 : 3;
    this->buttonSelection = 0;
    this->buttonSelected = false;
    this->timer = 0;

    this->state.Set(&PauseMenu::State_Paused);
    this->stateDraw.Init();

    this->animatorSelection.SetAnimation(modSVars->aniFrames, 0, true, 0);
    this->animatorPlate.SetAnimation(modSVars->aniFrames, PAUSEMENU_BUTTON_COUNT, true, this->inSpecialStage ? 2 : !sceneInfo->debugMode);
    this->animatorHighlight.SetAnimation(modSVars->aniFrames, PAUSEMENU_BUTTON_COUNT + 1, true, 0);

    // Begin pause
    Stage::SetEngineState(ENGINESTATE_FROZEN);
    this->PauseMusic();
    // TODO
    modSVars->sfxWoosh.Play(0, 1);

    for (int i = 0; i < PAUSEMENU_BUTTON_COUNT; ++i) {
        this->buttonActions[i].Init();
        this->buttonEnables[i] = true;
    }
 
    // Actions
    this->buttonActions[0].Set(&PauseMenu::Action_Continue);
    this->buttonActions[1].Set(&PauseMenu::Action_Restart);
    this->buttonActions[2].Set(&PauseMenu::Action_Exit);
    this->buttonActions[3].Set(&PauseMenu::Action_LevelSelect);

    // Disable on special stages
    if (this->inSpecialStage)
    {
        this->buttonEnables[1] = false;
        this->buttonEnables[2] = false;
        this->buttonEnables[3] = false;
        this->buttonCount = 1;
    }
}

void PauseMenu::Update(void) {
    if (sceneInfo->inEditor)
        return;

    // Open debug menu
    if (config.debugMenuEnable && controllerInfo[0].keyC.press)
        this->Reset(DebugMenu::sVars->classID, NULL);

    this->state.Run(this);
    this->timer++;
}

void PauseMenu::Draw(void) {

    // Copy palette
    if (sceneInfo->activeCategory == 5)
        paletteBank[0].Copy(7, 16 * 8, 0, 16 * 2);

    // Display animation
    if (this->buttonSelected && this->timer < modSVars->selectionAniDelay)
        this->animatorSelection.frameID = (this->timer / 2) % 2;
    else
        this->animatorSelection.frameID = 0;

    Vector2 pos;
    pos.x = TO_FIXED(screenInfo->size.x / 2);
    pos.y = TO_FIXED(screenInfo->size.y / 2);
    this->animatorPlate.DrawSprite(&pos, true);
    this->animatorHighlight.DrawSprite(&pos, true);
    this->animatorSelection.DrawSprite(&pos, true);

    stateDraw.Run(this);
}

void PauseMenu::StageLoad(void) {
    modSVars->sfxBleep.Get("Global/MenuBleep.wav");
    modSVars->sfxAccept.Get("Global/MenuAccept.wav");
    modSVars->sfxWoosh.Get("Global/MenuWoosh.wav");
    modSVars->aniFrames.Load("3K_Global/Pause.bin", SCOPE_STAGE);

    modSVars->selectionAniDuration = 60;
    modSVars->selectionAniDelay = 24;

    sVars->disableEvents = false;
}

void PauseMenu::StopMusic(void)
{
    for (int i = 0; i < CHANNEL_COUNT; ++i)
    {
        if (channels[i].IsActive())
            channels[i].Stop();
    }
}

void PauseMenu::PauseMusic(void)
{
    for (int i = 0; i < CHANNEL_COUNT; ++i)
    {        
        bool32 channelActive = channels[i].IsActive();

        modSVars->activeChannels[i] = channelActive;
        if (channelActive)
            channels[i].Pause();
    }
}

void PauseMenu::ResumeMusic(void)
{
    for (int i = 0; i < CHANNEL_COUNT; ++i)
    {
        if (modSVars->activeChannels[i])
            channels[i].Resume();
    }
}

void PauseMenu::Action_Continue(void)
{
    this->timer = 0;
    this->state.Set(&PauseMenu::State_Resuming);
}

void PauseMenu::Action_Restart(void)
{
    this->timer = 0;
    this->state.Set(&PauseMenu::State_Restarting);
    this->stateDraw.Set(&PauseMenu::Draw_FadeOut);
}

void PauseMenu::Action_Exit(void)
{
    this->timer = 0;
    this->state.Set(&PauseMenu::State_ReturningToTitle);
    this->stateDraw.Set(&PauseMenu::Draw_FadeOut);
}

void PauseMenu::Action_LevelSelect(void)
{
    this->timer = 0;
    this->state.Set(&PauseMenu::State_EnteringLevelSelect);
    this->stateDraw.Set(&PauseMenu::Draw_FadeOut);
}

void PauseMenu::Draw_FadeOut(void)
{
    float progress = ((this->timer - modSVars->selectionAniDelay) / (float)modSVars->selectionAniDuration) * 320.0f;
    int32 alpha = (int32)progress;
    Graphics::FillScreen(0x000000, alpha, alpha - 32, alpha - 64);
    if (alpha >= 320)
        this->fadeCompleted = true;
}

void PauseMenu::State_Paused(void)
{
    // Let one frame pass
    if (this->timer == 0)
        return;

    auto controller = &controllerInfo[0];
    bool32 selectionCanged = false;
    bool32 runAction = false;

    if (controller->keyA.press || controller->keyStart.press) {
        runAction = true;
    }
    else if (controller->keyUp.press) {
        modSVars->sfxBleep.Play(0, 1);
        do {
            --this->buttonSelection;

            if (this->buttonSelection < 0)
                this->buttonSelection = this->buttonCount - 1;

            selectionCanged = true;
        } while (!this->buttonEnables[this->buttonSelection]);
    }
    else if (controller->keyDown.press) {
        modSVars->sfxBleep.Play(0, 1);
        do {
            ++this->buttonSelection;

            if (this->buttonSelection >= this->buttonCount)
                this->buttonSelection = 0;

            selectionCanged = true;
        } while (!this->buttonEnables[this->buttonSelection]);
    }
    else {
        // Check touch
        auto center = screenInfo->center;

        for (int i = 0; i < PAUSEMENU_BUTTON_COUNT; ++i) {
            auto frame = modSVars->aniFrames.GetFrame(PAUSEMENU_BUTTON_COUNT + 1, i);
            if (this->buttonEnables[i]
                && Helpers::CheckTouch(
                center.x + frame->pivotX,
                center.y + frame->pivotY,
                frame->width, frame->height
            )) {
                this->buttonSelection = i;
                runAction = true;
                selectionCanged = true;
                break;
            }
        }
    }

    if (selectionCanged) {
        this->animatorSelection.SetAnimation(modSVars->aniFrames, this->buttonSelection, false, 0);
        this->animatorHighlight.SetAnimation(modSVars->aniFrames, PAUSEMENU_BUTTON_COUNT + 1, true, this->buttonSelection);
    }
    if (runAction) {
        this->buttonSelected = true;
        modSVars->sfxAccept.Play(0, 1);
        if (this->buttonActions[this->buttonSelection].state)
            this->buttonActions[this->buttonSelection].Run(this);
    }
}

void PauseMenu::State_Resuming(void)
{
    if (this->timer >= modSVars->selectionAniDelay)
    {
        Stage::SetEngineState(ENGINESTATE_REGULAR);
        this->ResumeMusic();
        this->Destroy();
    }
}

void PauseMenu::State_Restarting(void)
{
    if (this->fadeCompleted) {

        // Restart mission
        if (globals->playMode == BOOT_PLAYMODE_MISSION && modGlobals->missionData)
            modGlobals->startMission = true;

        Stage::LoadScene();
        this->StopMusic();
        this->Destroy();
    }
}

void PauseMenu::State_ReturningToTitle(void)
{
    if (this->fadeCompleted)
    {
        Stage::SetScene("Presentation & Menus", "Title Screen");
        Stage::LoadScene();
        this->StopMusic();
        this->Destroy();
    }
}

void PauseMenu::State_EnteringLevelSelect(void)
{
    if (this->fadeCompleted)
    {
        Stage::SetScene("Presentation & Menus", "Level Select");
        Stage::LoadScene();
        this->StopMusic();
        this->Destroy();
    }
}

} // namespace GameLogic