#include "../S3KExtensions.hpp"

using namespace RSDK;

namespace GameLogic {

// -------------
// Object States
// -------------

StateMachine<S3K_SaveMenu> S3K_SaveMenu::State_Init;

// ----------------------
// Extra Entity Functions
// ----------------------

bool32 S3K_SaveMenu::State_Init_Hook(bool32 skippedState) {
    if (GameObject::Get(SLOT_PAUSEMENU)->classID == NULL) {
        GameObject::Get(SLOT_PAUSEMENU)->Reset(OptionsMenu::sVars->classID, NULL);
    }
    return false;
}


} // namespace GameLogic