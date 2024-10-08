#pragma once
#include "GameAPI/Game.hpp"

using namespace RSDK;

namespace GameLogic {

struct Player : GameObject::Entity {

    // ----------------
    // Static Variables
    // ----------------

    struct Static : GameObject::Static { };

    struct ModStatic : GameObject::Static { };

    // ----------------
    // Entity Variables
    // ----------------

    int8 pathtracerInfo[0x70];
    int32 rings;
    int32 lives;
    int32 score;

    // -------------
    // Object States
    // -------------

    static StateMachine<Player> Input_Gamepad;

    // ----------------------
    // Extra Entity Functions
    // ----------------------

    static bool32 Input_Gamepad_Hook(bool32 skippedState);

    // -------------------
    // Static Declarations
    // -------------------

    MOD_DECLARE(Player)
};

} // namespace GameLogic