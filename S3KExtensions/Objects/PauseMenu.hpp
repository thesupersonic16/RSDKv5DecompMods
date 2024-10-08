#pragma once
#include "GameAPI/Game.hpp"

#define PAUSEMENU_BUTTON_COUNT (4)

using namespace RSDK;

namespace GameLogic {

struct PauseMenu : GameObject::Entity {

    // ----------------
    // Static Variables
    // ----------------

    struct Static : GameObject::Static {
        uint32 padding;
        bool32 disableEvents;
    };

    struct ModStatic : GameObject::Static {
        SoundFX sfxBleep;
        SoundFX sfxAccept;
        SoundFX sfxWoosh;
        SpriteAnimation aniFrames;
        bool32 activeChannels[CHANNEL_COUNT];
        int32 selectionAniDuration;
        int32 selectionAniDelay;
    };

    // ----------------
    // Entity Variables
    // ----------------

    Animator animatorPlate;
    Animator animatorSelection;
    Animator animatorHighlight;
    int32 timer;
    bool32 fadeCompleted;
    StateMachine<PauseMenu> state;
    StateMachine<PauseMenu> stateDraw;
    bool32 inSpecialStage;
    int32 buttonCount;
    int32 buttonSelection;
    bool32 buttonSelected;
    bool32 buttonEnables[PAUSEMENU_BUTTON_COUNT];
    StateMachine<PauseMenu> buttonActions[PAUSEMENU_BUTTON_COUNT];

    // ----------------------
    // Standard Entity Events
    // ----------------------

    void Create(void *data);
    void Update(void);
    void Draw(void);
    static void StageLoad(void);

    // ----------------------
    // Extra Entity Functions
    // ----------------------

    static void StopMusic(void);
    static void PauseMusic(void);
    static void ResumeMusic(void);


    // -------------
    // Object States
    // -------------

    void Action_Continue(void);
    void Action_Restart(void);
    void Action_Exit(void);
    void Action_LevelSelect(void);
    void Draw_FadeOut(void);
    void State_Paused(void);
    void State_Resuming(void);
    void State_Restarting(void);
    void State_ReturningToTitle(void);
    void State_EnteringLevelSelect(void);

    // -------------------
    // Static Declarations
    // -------------------

    MOD_DECLARE(PauseMenu)
};

} // namespace GameLogic