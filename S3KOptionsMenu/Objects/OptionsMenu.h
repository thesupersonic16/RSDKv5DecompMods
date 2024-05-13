#pragma once
#include <Game.h>
#include "../S3K.h"

// Object Class
typedef struct {
    RSDK_OBJECT
    uint16 sfxBleep;
    uint16 ani;
} ObjectOptionsMenu;

// Entity Class
typedef struct {
    RSDK_ENTITY
    Animator animator;
    StateMachine(state);
} EntityOptionsMenu;

// Object Struct
extern ObjectOptionsMenu* OptionsMenu;
extern StateMachine(S3K_SaveSlot_Create);

void OptionsMenu_Create(void* data);
void OptionsMenu_Update(void);
void OptionsMenu_Draw(void);
void OptionsMenu_StageLoad(void);

void OptionsMenu_State_Menu(void);