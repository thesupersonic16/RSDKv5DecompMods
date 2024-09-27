#pragma once
#include "GameAPI/Game.hpp"

using namespace RSDK;

namespace GameLogic {

struct S3K_SaveSlot : GameObject::Entity {

    // ----------------
    // Static Variables
    // ----------------

    struct Static : GameObject::Static {};

    struct ModStatic : GameObject::Static {};

    // ----------------
    // Entity Variables
    // ----------------

    // ----------------------
    // Standard Entity Events
    // ----------------------

    // -------------
    // Object States
    // -------------

    // ----------------------
    // Extra Entity Functions
    // ----------------------

    // -------------------
    // Static Declarations
    // -------------------

    MOD_DECLARE(S3K_SaveSlot)
};

// ----------------------
// Extra Entity Functions
// ----------------------

} // namespace GameLogic