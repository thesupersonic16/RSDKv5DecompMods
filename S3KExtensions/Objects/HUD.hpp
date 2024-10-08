#pragma once
#include "GameAPI/Game.hpp"

#define PAUSEMENU_BUTTON_COUNT (4)

using namespace RSDK;

namespace GameLogic {

struct HUD : GameObject::Entity {

    // ----------------
    // Static Variables
    // ----------------

    struct Static : GameObject::Static {
        SpriteAnimation aniFrames;
        // ...
    };

    struct ModStatic : GameObject::Static { };

    // ----------------
    // Entity Variables
    // ----------------

    StateMachine<HUD> state;
    Vector2 scorePos;
    Vector2 timePos;
    Vector2 ringsPos;
    Vector2 lifePos;
    // ...

    // ----------------------
    // Extra Entity Functions
    // ----------------------

    static void (*DrawNumbersBase10)(HUD *self, Animator *animator, Vector2 *drawPos, int32 value, int32 digitCount);
    static void (*DrawNumbersBase16)(HUD *self, Vector2 *drawPos, int32 value);

    // -------------------
    // Static Declarations
    // -------------------

    MOD_DECLARE(HUD)
};

} // namespace GameLogic