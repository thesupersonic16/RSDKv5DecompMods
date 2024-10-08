#pragma once
#include "GameAPI/Game.hpp"

using namespace RSDK;

namespace GameLogic {

struct SignPost2 : GameObject::Entity {

    enum SignPostTypes : uint8 {
        SIGNPOST_DROP,
        SIGNPOST_RUNPAST,
        SIGNPOST_BOUNDING
    };

    // ----------------
    // Static Variables
    // ----------------

    struct Static : GameObject::Static {
        SpriteAnimation aniFrames;
        Hitbox hitboxSignPost;
        Hitbox hitboxItemBox;
        SoundFX sfxSignPost;
        SoundFX sfxSignPost2P;
        SoundFX sfxTwinkle;
        SoundFX sfxBubbleBounce;
        SoundFX sfxSlide;
        uint8 maxPlayerCount;
    };

    struct ModStatic : GameObject::Static {};

    // ----------------
    // Entity Variables
    // ----------------

    StateMachine<SignPost2> state;
    SignPostTypes type;

    // -------------
    // Object States
    // -------------

    static StateMachine<SignPost2> State_Falling;

    // -------------------
    // Static Declarations
    // -------------------

    MOD_DECLARE(SignPost2)

};

} // namespace GameLogic