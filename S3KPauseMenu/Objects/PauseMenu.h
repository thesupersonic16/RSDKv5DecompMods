#pragma once
#include <Game.h>

#define PAUSEMENU_BUTTON_COUNT (4)

// Object Class
typedef struct {
    RSDK_OBJECT
    uint16 sfxBleep;
    uint16 sfxAccept;
    bool32 disableEvents;
    uint16 sfxWoosh;
    uint16 aniPause;
    bool32 activeChannels[CHANNEL_COUNT];
} ObjectPauseMenu;

// Entity Class
typedef struct {
    RSDK_ENTITY
    Animator animatorPlate;
    Animator animatorSelection;
    Animator animatorHighlight;
    int32 timer;
    bool32 fadeCompleted;
    StateMachine(state);
    StateMachine(stateDraw);
    int32 buttonCount;
    int32 buttonSelection;
    bool32 buttonSelected;
    bool32 buttonEnables[PAUSEMENU_BUTTON_COUNT];
    void (*buttonActions[PAUSEMENU_BUTTON_COUNT])(void);
} EntityPauseMenu;

// Object Struct
extern ObjectPauseMenu* PauseMenu;

void PauseMenu_PauseMusic();
void PauseMenu_ResumeMusic();
void PauseMenu_Create(void* data);
void PauseMenu_Update(void);
void PauseMenu_Draw(void);
void PauseMenu_StageLoad(void);
void PauseMenu_StaticUpdate(void);
void PauseMenu_StaticLoad(ObjectPauseMenu*);
void PauseMenu_State_Pausing(void);
void PauseMenu_State_Resuming(void);
void PauseMenu_State_Restarting(void);
void PauseMenu_State_ReturningToTitle(void);
void PauseMenu_State_EnteringLevelSelect(void);
void PauseMenu_State_Paused(void);
void PauseMenu_Draw_FadeOut(void);
void PauseMenu_Action_Continue(void);
void PauseMenu_Action_Restart(void);
void PauseMenu_Action_Exit(void);
void PauseMenu_Action_LevelSelect(void);

