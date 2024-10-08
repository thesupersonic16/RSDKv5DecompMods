#pragma once
#include "GameAPI/Game.hpp"

using namespace RSDK;

namespace GameLogic {

struct Mission : GameObject::Entity {

    // Pulled from S3KT/Sonic3/GameVariables.hpp
    enum MissionFunctions {
        MISSIONNO_NONE            = 0,
        MISSIONNO_MERCY           = 8,
        MISSIONNO_RINGCHALLENGE50 = 10,
        MISSIONNO_AERIALATTACK    = 18,
        MISSIONNO_RINGVACCUM      = 30,
        MISSIONNO_BALLOONBURST    = 38,
        MISSIONNO_FIREBALLDASH    = 39,
    };

    // ----------------
    // Static Variables
    // ----------------

    struct Static : GameObject::Static {
        SoundFX sfxAccept;
    };

    // ----------------
    // Entity Variables
    // ----------------

    Animator hudElementsAnimator;
    Animator numbersAnimator;
    Animator lifeIconAnimator;
    Animator lifeNamesAnimator;
    Animator lifeNumbersAnimator;
    StateMachine<Mission> state;
    StateMachine<Mission> stateDrawHud;
    StateMachine<Mission> stateDrawMenu;
    StateMachine<Mission> stateDrawForeground;
    int32 timer;
    bool32 fadeCompleted;
    bool32 objectiveCompleted;
    bool32 displayMeters;

    // ----------------------
    // Standard Entity Events
    // ----------------------

    void Create(void *data);
    void Update(void);
    void Draw(void);
    static void StageLoad(void);

    // ----------------------
    // Extra Entity Functions
    // ----------------------

    void SetCompleted(void);
    void SetPlayerDeath(void);
    void DropSignPost(void);
    bool32 CheckCondition(int32 id);
    bool32 IsObjectiveCompleted(void);
    static void StartMission(Origins::MissionData *data, Mission::MissionFunctions objective);

    // -------------
    // Object States
    // -------------

    void Draw_FadeOut(void);
    void Draw_HUD(void);
    void State_Playing();
    void State_Retry();
    void State_Finished();
    void State_Exiting();
    void State_Retrying();

    // -------------------
    // Static Declarations
    // -------------------

    RSDK_DECLARE(Mission)
};

} // namespace GameLogic