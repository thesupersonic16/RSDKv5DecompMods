#include "../S3KExtensions.hpp"

using namespace RSDK;

namespace GameLogic {

// -------------------
// Object Registration
// -------------------

RSDK_REGISTER_OBJECT(Mission);

// ----------------------
// Standard Entity Events
// ----------------------

void Mission::Create(void *data) {
    if (sceneInfo->inEditor)
        return;

    this->active = ACTIVE_ALWAYS;
    this->visible = true;
    // TODO: Use Zone::sVars->hudDrawGroup
    this->drawGroup = 14;
    this->timer = 0;
    this->objectiveCompleted = false;
    this->displayMeters = true;
    this->state.Set(&Mission::State_Playing);
    this->stateDrawHud.Init();
    this->stateDrawMenu.Init();
    this->stateDrawForeground.Init();

    globals->missionComplete = false;
    modGlobals->startMission = false;
    for (int i = 0; i < 3; ++i)
        modGlobals->missionProgress[i] = 0;

    // It's a good thing we load after HUD
    auto hud = Helpers::FindFirstEntity<HUD>();
    if (hud) {
        if (hud->scorePos.y == TO_FIXED(-64))
            this->displayMeters = false; // Ultrafix
        else {
            // Shift HUD positions
            hud->ringsPos = hud->timePos;
            hud->timePos  = hud->scorePos;
        }
    }

    // HUD
    this->hudElementsAnimator.SetAnimation(HUD::sVars->aniFrames, 0, true, 0);
    this->numbersAnimator.SetAnimation(HUD::sVars->aniFrames, 1, true, 0);
    this->lifeIconAnimator.SetAnimation(HUD::sVars->aniFrames, 2, true, 6);
    this->lifeNamesAnimator.SetAnimation(HUD::sVars->aniFrames, 15, true, 5);
    this->lifeNumbersAnimator.SetAnimation(HUD::sVars->aniFrames, 14, true, 0);

}

void Mission::Update(void) {
    if (globals->playMode != BOOT_PLAYMODE_MISSION && modGlobals->missionData)
        return;
    this->timer++;
    this->state.Run(this);
}

void Mission::Draw(void) {
    if (globals->playMode != BOOT_PLAYMODE_MISSION)
        return;

    Vector2 drawPos;
    drawPos.x = 16;
    drawPos.y = 2 + 40;
    DrawHelpers::DrawDevStringWithShadow("Mission Mode", drawPos.x, drawPos.y, ALIGN_LEFT, 0xE0E0E0);
    drawPos.y += 8;

    auto player = GameObject::Get<Player>(SLOT_PLAYER1);
    char str[128] {};
    
    for (int i = 0; i < 3; ++i) {
        auto condition = modGlobals->missionData->m_conditions[i];
        if (condition.m_type == Origins::NOP)
            continue;
        const char* formatStr = "NONE";
        switch (condition.m_type) {
            case Origins::RING_COUNT:
                formatStr = "[%d] Rings: %d/%d";
                break;
            case Origins::OBJECT_COUNT:
                formatStr = "[%d] Objects: %d/%d";
                break;
            case Origins::DAMAGE_COUNT:
                formatStr = "[%d] Damages: %d/%d";
                break;
            case Origins::SCORE_COUNT:
                formatStr = "[%d] Score: %d/%d";
                break;
            case Origins::ANIMAL_BOX_COUNT:
                formatStr = "[%d] Animal Boxes: %d/%d";
                break;
        }
        sprintf_s(str, formatStr, i, modGlobals->missionProgress[i], condition.m_value);
        DrawHelpers::DrawDevStringWithShadow(str, drawPos.x, drawPos.y, ALIGN_LEFT, CheckCondition(i) ? 0x10E010 : 0xE0E0E0);
        drawPos.y += 8;
    }

    if (this->objectiveCompleted) {
        DrawHelpers::DrawDevStringWithShadow("Objective Complete", drawPos.x, drawPos.y, ALIGN_LEFT, 0x10E010);
        drawPos.y += 8;
    }


    uint32 seconds = sceneInfo->seconds + sceneInfo->minutes * 60;
    const char *ranks = "SABCD";
    int rank = 0;
    for (int i = 0; i < 4; ++i) {
        int32 rankTime = modGlobals->missionData->m_rankTimes[i];
        if (rankTime != 0 && seconds >= modGlobals->missionData->m_rankTimes[i])
            ++rank;

    }
    if (modGlobals->missionData->m_timeLimitSec)
        sprintf_s(str, "Time Remaining: %d (%c)", modGlobals->missionData->m_timeLimitSec - seconds, ranks[rank]);
    else
        sprintf_s(str, "Time: %d (%c)", seconds, ranks[rank]);
    DrawHelpers::DrawDevStringWithShadow(str, drawPos.x, drawPos.y, ALIGN_LEFT, 0xE0E0E0);
    drawPos.y += 8;

    // State messages
    if (this->state.Matches(&Mission::State_Finished))
    {
        DrawHelpers::DrawDevStringWithShadow("Return to Title: A/Start", drawPos.x, drawPos.y, ALIGN_LEFT, 0xE0E0E0);
        drawPos.y += 8;
    } 
    else if (this->state.Matches(&Mission::State_Retry))
    {
        DrawHelpers::DrawDevStringWithShadow("Retry: A/Start", drawPos.x, drawPos.y, ALIGN_LEFT, 0xE0E0E0);
        drawPos.y += 8;
        DrawHelpers::DrawDevStringWithShadow("Return to Title: B", drawPos.x, drawPos.y, ALIGN_LEFT, 0xE0E0E0);
        drawPos.y += 8;
    }

    this->Draw_HUD();

    // Got no graphics yet :(
    stateDrawForeground.Run(this);
}

void Mission::StageLoad(void) {
    Dev::Print(Dev::PRINT_NORMAL, "Mission::StageLoad");
    sVars->sfxAccept.Get("Global/MenuAccept.wav");
}

void Mission::DropSignPost(void) {
    globals->missionComplete = true;
}

void Mission::SetCompleted(void) {
    if (!modGlobals->missionData)
        return;
    if (this->objectiveCompleted)
        this->state.Set(&Mission::State_Finished);
    else
        this->state.Set(&Mission::State_Retry);
    PauseMenu::sVars->disableEvents = true;
    GameObject::Get(SLOT_ZONE)->active = ACTIVE_NEVER;
}

void Mission::SetPlayerDeath(void) {
    if (!modGlobals->missionData)
        return;
    this->state.Set(&Mission::State_Retry);
    PauseMenu::sVars->disableEvents = true;
    GameObject::Get(SLOT_ZONE)->active = ACTIVE_NEVER;
}

bool32 Mission::CheckCondition(int32 id) {
    if (!modGlobals->missionData)
        return false;

    auto condition = modGlobals->missionData->m_conditions[id % 3];
    switch (condition.m_opCode) {
    case Origins::OpCode_NONE:
        return true;
    case Origins::EQUAL:
        return modGlobals->missionProgress[id % 3] == condition.m_value;
    case Origins::GREATER_EQUAL:
        return modGlobals->missionProgress[id % 3] >= condition.m_value;
    default:
        return false;
    }
}

bool32 Mission::IsObjectiveCompleted(void) {
    bool32 complete = true;
    for (int i = 0; i < 3; ++i)
        if (!CheckCondition(i))
            complete = false;
    return complete;
}

void Mission::StartMission(Origins::MissionData* data, Mission::MissionFunctions objective) {
    Dev::Print(Dev::PRINT_NORMAL, "Mission::StartMission");
    modGlobals->missionData = data;
    modGlobals->startMission = true;

    globals->missionFunctionNo = objective;

    // Select scene
    auto category = sceneInfo->listCategory[4];
    sceneInfo->listPos = category.sceneOffsetStart + data->m_stageListPos;
    sceneInfo->activeCategory = 4;
    globals->playMode = BOOT_PLAYMODE_MISSION;
    globals->playerID = 1 << data->m_playerType;
    globals->playerID |= data->m_enableTails ? ID_TAILS_ASSIST : ID_NONE;
    globals->medalMods &= ~MEDAL_ANDKNUCKLES;

    Stage::LoadScene();
}

void Mission::Draw_FadeOut(void) {
    float progress = (this->timer / 60.0f) * 320.0f;
    int32 alpha = (int32)progress;
    Graphics::FillScreen(0x000000, alpha, alpha - 32, alpha - 64);
    if (alpha >= 320)
        this->fadeCompleted = true;
}

void Mission::Draw_HUD(void) {
    auto hud = Helpers::FindFirstEntity<HUD>();
    auto player = GameObject::Get<Player>(sceneInfo->currentScreenID);
    Vector2 drawPos;
    this->numbersAnimator.frameID = 0;
    int32 numberHeight = this->numbersAnimator.GetFrame(HUD::sVars->aniFrames)->height;

    if (this->displayMeters) {
        // Time
        drawPos = hud->timePos;
        this->hudElementsAnimator.frameID = 1;
        this->hudElementsAnimator.DrawSprite(&drawPos, true);
        this->hudElementsAnimator.frameID = 20;
        this->hudElementsAnimator.DrawSprite(&drawPos, true);
        drawPos.x += TO_FIXED(75);
        drawPos.y += TO_FIXED(numberHeight);
        HUD::DrawNumbersBase10(hud, &this->numbersAnimator, &drawPos, sceneInfo->minutes > 9 ? 59 : sceneInfo->seconds, 2);
        drawPos.x -= 0x80000;
        HUD::DrawNumbersBase10(hud, &this->numbersAnimator, &drawPos, CLAMP(sceneInfo->minutes, 0, 9), 1);

        // Rings
        drawPos = hud->ringsPos;
        this->hudElementsAnimator.frameID = 3;
        this->hudElementsAnimator.DrawSprite(&drawPos, true);
        this->numbersAnimator.frameID = 0;
        drawPos.x += TO_FIXED(75);
        drawPos.y += TO_FIXED(numberHeight);
        HUD::DrawNumbersBase10(hud, &this->numbersAnimator, &drawPos, player->rings, 0);
    }

    if (sceneInfo->debugMode) {
        // Using screen 0 for now
        // Draw Camera YPos
        drawPos.x = TO_FIXED(screenInfo[0].size.x - 16);
        drawPos.y = TO_FIXED(24);
        HUD::DrawNumbersBase16(hud, &drawPos, screenInfo[0].position.y);

        // Draw Camera XPos
        drawPos.x -= TO_FIXED(9);
        HUD::DrawNumbersBase16(hud, &drawPos, screenInfo[0].position.x);

        // Draw Player YPos
        drawPos.x = TO_FIXED(screenInfo[0].size.x - 16);
        drawPos.y += TO_FIXED(16);
        HUD::DrawNumbersBase16(hud, &drawPos, FROM_FIXED(player->position.y));

        // Draw Player XPos
        drawPos.x -= TO_FIXED(9);
        HUD::DrawNumbersBase16(hud, &drawPos, FROM_FIXED(player->position.x));
    }

    // Life section
    drawPos = hud->lifePos;
    this->lifeIconAnimator.DrawSprite(&drawPos, true);
    this->lifeNamesAnimator.DrawSprite(&drawPos, true);
    this->hudElementsAnimator.frameID = 14;

    drawPos.x -= TO_FIXED(2);
    this->hudElementsAnimator.DrawSprite(&drawPos, true);
    drawPos.x += TO_FIXED(44);
    HUD::DrawNumbersBase10(hud, &this->lifeNumbersAnimator, &drawPos, globals->coinCount, 0);
 }

// Mission started
void Mission::State_Playing() {
    SET_CURRENT_STATE();

    if (IsObjectiveCompleted() && !this->objectiveCompleted) {
        this->objectiveCompleted = true;
        this->timer = 0;
        DropSignPost();
    }

    // Use rings if no function is set
    if (globals->missionFunctionNo == MISSIONNO_NONE)
        modGlobals->missionProgress[0] = GameObject::Get<Player>(SLOT_PLAYER1)->rings;
}

// Retry upon death
void Mission::State_Retry() {
    SET_CURRENT_STATE();

    // Add delay
    if (this->timer < 30)
        return;

    if (controllerInfo[0].keyA.press || controllerInfo[0].keyStart.press) {
        sVars->sfxAccept.Play(0, 1);
        this->timer = 0;
        this->state.Set(&Mission::State_Retrying);
        this->stateDrawForeground.Set(&Mission::Draw_FadeOut);
    }
    if (controllerInfo[0].keyB.press) {
        sVars->sfxAccept.Play(0, 1);
        this->timer = 0;
        this->state.Set(&Mission::State_Exiting);
        this->stateDrawForeground.Set(&Mission::Draw_FadeOut);
    }
}

// Results
void Mission::State_Finished() {
    SET_CURRENT_STATE();

    // Add delay
    if (this->timer < 30)
        return;

    if (controllerInfo[0].keyA.press || controllerInfo[0].keyStart.press) {
        sVars->sfxAccept.Play(0, 1);
        this->timer = 0;
        this->state.Set(&Mission::State_Exiting);
        this->stateDrawForeground.Set(&Mission::Draw_FadeOut);
    }
}

// Transition
void Mission::State_Exiting() {
    SET_CURRENT_STATE();
    
    if (this->fadeCompleted) {
        modGlobals->missionData = nullptr;
        Stage::SetScene("Presentation & Menus", "Title Screen");
        Stage::LoadScene();
    }
}

// Transition
void Mission::State_Retrying() {
    SET_CURRENT_STATE();
    
    if (this->fadeCompleted) {
        modGlobals->startMission = true;
        Stage::LoadScene();
    }
}

} // namespace GameLogic