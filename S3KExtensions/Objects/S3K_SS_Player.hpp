#pragma once
#include "GameAPI/Game.hpp"

using namespace RSDK;

namespace GameLogic {

struct S3K_SS_Player : GameObject::Entity {

    // -------------
    // Object States
    // -------------

    static StateMachine<S3K_SS_Player> Input_Gamepad;

    // ----------------------
    // Extra Entity Functions
    // ----------------------

    static bool32 Input_Gamepad_Hook(bool32 skippedState);
};

} // namespace GameLogic