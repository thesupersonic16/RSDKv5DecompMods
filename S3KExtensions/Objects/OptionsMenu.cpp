#include "../S3KExtensions.hpp"

using namespace RSDK;

namespace GameLogic {

// -------------------
// Object Registration
// -------------------

RSDK_REGISTER_OBJECT(OptionsMenu);

// ----------------------
// Standard Entity Events
// ----------------------

void OptionsMenu::Create(void *data) {
    if (sceneInfo->inEditor)
        return;

    this->active = ACTIVE_ALWAYS;
    this->visible = true;
    this->drawGroup = 14;
    this->state.Set(&OptionsMenu::State_Menu);

    for (int i = 0; i < _countof(this->touch); ++i)
        this->touch[i].down = this->touch[i].press = false;
}

void OptionsMenu::LateUpdate(void) {
    for (int i = 0; i < _countof(this->touch); ++i)
        this->touch[i].press = false;
}

void OptionsMenu::Update(void) {
    if (sceneInfo->inEditor)
        return;

    this->state.Run(this);
}

void OptionsMenu::Draw(void) {
    Vector2 screenSize = screenInfo->size;
    bool32 isSmall = screenSize.x < 330;

    // Sprite positions
    Vector2 posGameIcon, posGameHeader, posGameOption;
    Vector2 posMirrorIcon, posMirrorHeader, posMirrorOption;

    if (isSmall) {
        SetTouchDown(0, Helpers::CheckTouch(8, screenSize.y - 40, 88, 32));
        posGameIcon.x = TO_FIXED(8);
        posGameIcon.y = TO_FIXED(screenSize.y - 40);
        posGameHeader.x = TO_FIXED(8);
        posGameHeader.y = TO_FIXED(screenSize.y - 23);
        posGameOption.x = TO_FIXED(8);
        posGameOption.y = TO_FIXED(screenSize.y - 15);

        if (sVars->leftAlign) {
            SetTouchDown(1, Helpers::CheckTouch(8, screenSize.y - 74, 88, 32));
            posMirrorIcon.x = TO_FIXED(8);
            posMirrorIcon.y = TO_FIXED(screenSize.y - 74);
            posMirrorHeader.x = TO_FIXED(8);
            posMirrorHeader.y = TO_FIXED(screenSize.y - 57);
            posMirrorOption.x = TO_FIXED(8);
            posMirrorOption.y = TO_FIXED(screenSize.y - 49);
        }
        else {
            SetTouchDown(1, Helpers::CheckTouch(screenSize.x - 96, screenSize.y - 40, 88, 32));
            posMirrorIcon.x = TO_FIXED(screenSize.x - 24);
            posMirrorIcon.y = TO_FIXED(screenSize.y - 40);
            posMirrorHeader.x = TO_FIXED(screenSize.x - 96);
            posMirrorHeader.y = TO_FIXED(screenSize.y - 23);
            posMirrorOption.x = TO_FIXED(screenSize.x - 8);
            posMirrorOption.y = TO_FIXED(screenSize.y - 15);
        }
    }
    else {
        SetTouchDown(0, Helpers::CheckTouch(8, screenSize.y - 24, 105, 16));
        posGameIcon.x = TO_FIXED(8);
        posGameIcon.y = TO_FIXED(screenSize.y - 24);
        posGameHeader.x = TO_FIXED(25);
        posGameHeader.y = TO_FIXED(screenSize.y - 23);
        posGameOption.x = TO_FIXED(25);
        posGameOption.y = TO_FIXED(screenSize.y - 15);

        if (sVars->leftAlign) {
            SetTouchDown(1, Helpers::CheckTouch(8, screenSize.y - 42, 105, 16));
            posMirrorIcon.x = TO_FIXED(8);
            posMirrorIcon.y = TO_FIXED(screenSize.y - 42);
            posMirrorHeader.x = TO_FIXED(25);
            posMirrorHeader.y = TO_FIXED(screenSize.y - 41);
            posMirrorOption.x = TO_FIXED(25);
            posMirrorOption.y = TO_FIXED(screenSize.y - 33);
        }
        else {
            SetTouchDown(1, Helpers::CheckTouch(screenSize.x - 113, screenSize.y - 24, 105, 16));
            posMirrorIcon.x = TO_FIXED(screenSize.x - 24);
            posMirrorIcon.y = TO_FIXED(screenSize.y - 24);
            posMirrorHeader.x = TO_FIXED(screenSize.x - 113);
            posMirrorHeader.y = TO_FIXED(screenSize.y - 23);
            posMirrorOption.x = TO_FIXED(screenSize.x - 25);
            posMirrorOption.y = TO_FIXED(screenSize.y - 15);
        }
    }

    // Game Mode Icon
    this->animator.SetAnimation(sVars->aniFrames, 0, true, globals->playMode != 0);
    this->animator.DrawSprite(&posGameIcon, true);
    // Game Mode Header
    this->animator.SetAnimation(sVars->aniFrames, 1, true, 0);
    this->animator.DrawSprite(&posGameHeader, true);
    // Game Mode Option
    this->animator.SetAnimation(sVars->aniFrames, 2, true, globals->playMode != 0);
    this->animator.DrawSprite(&posGameOption, true);

    // Mirror Mode icon
    this->animator.SetAnimation(sVars->aniFrames, 0, true, (globals->unknown4C34D8 == 0) + 2);
    this->animator.DrawSprite(&posMirrorIcon, true);
    // Mirror Mode Header
    this->animator.SetAnimation(sVars->aniFrames, 1, true, 1);
    this->animator.DrawSprite(&posMirrorHeader, true);
    // Mirror Mode Option
    this->animator.SetAnimation(sVars->aniFrames, 2, true, (globals->unknown4C34D8 == 0) + 2);

    // Move frame right if in left align mode
    if (sVars->leftAlign)
        posMirrorOption.x += TO_FIXED(sVars->aniFrames.GetFrame(2, (globals->unknown4C34D8 == 0) + 2)->width);

    this->animator.DrawSprite(&posMirrorOption, true);
}

void OptionsMenu::StageLoad(void) {
    sVars->sfxBleep.Get("Stage/Button.wav");
    sVars->aniFrames.Load("3K_Menu/OptionsMenu.bin", SCOPE_STAGE);
    sVars->leftAlign = true;
}

void OptionsMenu::SetTouchDown(int32 id, bool32 state)
{
    if (!this->touch[id].down && state)
        this->touch[id].press = true;
    this->touch[id].down = state;
}

void OptionsMenu::State_Menu()
{
    SET_CURRENT_STATE();

    auto controller = &controllerInfo[0];
    bool32 updateSaveSlots = false;

    // Game Mode
    if (controller->keyC.press || this->touch[0].press) {
        sVars->sfxBleep.Play(0, 0);
        globals->playMode ^= 1;
        globals->playMode &= 1; // Remove other flags like mission mode

        if (globals->playMode & 1) {
            globals->disableLives = true;
            globals->useCoins = true;
        }
        else {
            globals->disableLives = false;
            globals->useCoins = false;
            globals->unknown4C34D8 = false;
        }
        updateSaveSlots = true;
    }

    // Mirror Mode
    if (controller->keyZ.press|| this->touch[1].press) {
        sVars->sfxBleep.Play(0, 0);
        globals->unknown4C34D8 ^= 1;

        if (globals->unknown4C34D8)
            globals->playMode = 3;
        else
            globals->playMode = 1;

        globals->disableLives = true;
        globals->useCoins = true;

        updateSaveSlots = true;
    }

    if (updateSaveSlots) {
        auto thisEntity = this;
        for (auto slot : GameObject::GetEntities<S3K_SaveSlot>(FOR_ALL_ENTITIES)) {
            sceneInfo->entity = slot; // Why must I have to do this?
            slot->sVars->Super(SUPER_CREATE, nullptr);
        }
        sceneInfo->entity = thisEntity;        
    }
}


} // namespace GameLogic