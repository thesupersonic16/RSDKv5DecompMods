#include "../S3KExtensions.hpp"

using namespace RSDK;

namespace GameLogic {

// -------------
// Object States
// -------------

StateMachine<Player> Player::Input_Gamepad;

// ----------------------
// Extra Entity Functions
// ----------------------

bool32 Player::Input_Gamepad_Hook(bool32 skippedState)
{
    if (sceneInfo->state == ENGINESTATE_REGULAR
        && controllerInfo[0].keyStart.press
        && !PauseMenu::sVars->disableEvents
        && GameObject::Get(SLOT_PLAYER1)->classID != 0
        && GameObject::Get(SLOT_ACTCLEAR)->classID == 0
        && GameObject::Get(SLOT_PAUSEMENU)->classID == 0)
    {
        GameObject::Reset(SLOT_PAUSEMENU, PauseMenu::sVars->classID, NULL);
    }
    return false;
}

} // namespace GameLogic