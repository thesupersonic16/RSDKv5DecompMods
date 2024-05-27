#pragma once
#include <Game.h>
#include "../S3K.h"
#include "../Helpers.h";

// Object Class
typedef struct {
    RSDK_OBJECT
    uint16 sfxBleep;
    uint16 ani;
    bool32 leftAlign;
} ObjectOptionsMenu;

// Entity Class
typedef struct {
    RSDK_ENTITY
    Animator animator;
    StateMachine(state);
    InputState touch[2];
} EntityOptionsMenu;

// Object Struct
extern ObjectOptionsMenu* OptionsMenu;
extern StateMachine(S3K_SaveSlot_Create);

void OptionsMenu_Create(void* data);
void OptionsMenu_LateUpdate(void);
void OptionsMenu_Update(void);
void OptionsMenu_Draw(void);
void OptionsMenu_StageLoad(void);

void OptionsMenu_State_Menu(void);