#pragma once
#include "GameAPI/Game.hpp"

using namespace RSDK;

namespace GameLogic {

struct OptionsMenu : GameObject::Entity {

    // ----------------
    // Static Variables
    // ----------------

    struct Static : GameObject::Static {
        SoundFX sfxBleep;
        SpriteAnimation aniFrames;
        bool32 leftAlign;
    };

    // ----------------
    // Entity Variables
    // ----------------

    Animator animator;
    StateMachine<OptionsMenu> state;
    InputState touch[2];

    // ----------------------
    // Standard Entity Events
    // ----------------------

    void Create(void *data);
    void LateUpdate(void);
    void Update(void);
    void Draw(void);
    static void StageLoad(void);

    // ----------------------
    // Extra Entity Functions
    // ----------------------

    void SetTouchDown(int32 id, bool32 state);

    // -------------
    // Object States
    // -------------

    void State_Menu(void);

    // -------------------
    // Static Declarations
    // -------------------

    RSDK_DECLARE(OptionsMenu)
};

} // namespace GameLogic