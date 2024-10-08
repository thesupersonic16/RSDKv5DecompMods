#pragma once
#include "GameAPI/Game.hpp"

using namespace RSDK;

namespace GameLogic {

struct DrawHelpers : GameObject::Entity {

    // ----------------------
    // Extra Entity Functions
    // ----------------------

    static void DrawDevString(const char* string, int32 x, int32 y, int32 align, uint32 color);
    static void DrawDevStringWithShadow(const char* string, int32 x, int32 y, int32 align, uint32 color);

};
} // namespace GameLogic