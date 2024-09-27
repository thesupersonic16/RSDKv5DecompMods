#include "../S3KExtensions.hpp"

using namespace RSDK;

namespace GameLogic {

// -------------
// Object States
// -------------

StateMachine<S3K_SS_Player> S3K_SS_Player::Input_Gamepad;

// ----------------------
// Extra Entity Functions
// ----------------------

bool32 S3K_SS_Player::Input_Gamepad_Hook(bool32 skippedState)
{
    // Reuse hooking code from Player::Input_Gamepad_Hook
    return Player::Input_Gamepad_Hook(skippedState);
}

} // namespace GameLogic