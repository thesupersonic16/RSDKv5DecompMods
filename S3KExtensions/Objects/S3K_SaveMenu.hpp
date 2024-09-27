#pragma once
#include "GameAPI/Game.hpp"

using namespace RSDK;

namespace GameLogic {

struct S3K_SaveMenu : GameObject::Entity {

    // -------------
    // Object States
    // -------------
    static StateMachine<S3K_SaveMenu> State_Init;

    // ----------------------
    // Extra Entity Functions
    // ----------------------

    static bool32 State_Init_Hook(bool32 skippedState);
};

} // namespace GameLogic