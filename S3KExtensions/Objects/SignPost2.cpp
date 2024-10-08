#include "../S3KExtensions.hpp"

using namespace RSDK;

namespace GameLogic {

    // -------------
    // Object States
    // -------------

    StateMachine<SignPost2> SignPost2::State_Falling;

    // -------------------
    // Object Registration
    // -------------------

    MOD_REGISTER_OBJECT(SignPost2);

} // namespace GameLogic