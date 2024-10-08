#include "../S3KExtensions.hpp"

using namespace RSDK;

namespace GameLogic {

    // ----------------
    // Public Functions
    // ----------------

    void (*HUD::DrawNumbersBase10)(HUD *self, Animator *animator, Vector2 *drawPos, int32 value, int32 digitCount);
    void (*HUD::DrawNumbersBase16)(HUD *self, Vector2 *drawPos, int32 value);

    // -------------------
    // Object Registration
    // -------------------

    MOD_REGISTER_OBJECT(HUD);

} // namespace GameLogic