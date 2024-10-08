#pragma once
#include "GameAPI/Game.hpp"

enum S3PlayModes {
    BOOT_PLAYMODE_CLASSIC,
    BOOT_PLAYMODE_ANNIVERSARY,
    BOOT_PLAYMODE_BOSSRUSH,
    BOOT_PLAYMODE_MIRRORING,
    BOOT_PLAYMODE_MISSION,
    BOOT_PLAYMODE_STORY,
};

// Mod
struct S3KExtensionsConfig {
    bool pauseMenuEnable;
    bool optionsMenuEnable;
    bool missionHandlerEnable;
    bool debugMenuEnable;
};

// Variables
struct S3KGlobalVariables {
    uint8 gameMode;
    uint32 playerID;
    uint8 gap8[0xC2CAC];
    uint32 medalMods;
    uint32 gapC2CB8[0x1001D2];
    uint32 playerSpriteStyle;
    uint32 gameSpriteStyle;
    uint32 ostStyle;
    uint8 gap4C340C[0xC8];
    bool32 disableLives;
    int32 unknown4C34D8;
    int32 unknown4C34DC;
    int32 coinCount;
    uint8 gap4C34DC[0x28];
    bool32 hasPlusDLC;
    int32 playMode;
    int32 unknown4C3514;
    int32 unknown4C3518;
    int32 unknown4C351C;
    int32 unknown4C3520;
    int32 unknown4C3524;
    bool32 useCoins;
    uint8 gap4C352C[0x02];
    bool32 missionComplete;
    int32 missionFunctionNo;
    uint8 gap4C3538[0x54];
//    uint8 gap4C352C[0x60];
    bool32 waitSSRetry;
};


// Other
#include "Callbacks.hpp"
#include "Helpers.hpp"
#include "MissionInfo.hpp"

struct S3KExtensionsGlobalVariables {
    Origins::MissionInfo* misionInfo;
    Origins::MissionData* missionData;
    int32 missionProgress[3];
    bool32 startMission;
    int32 missionRFL[0x1000];
};

extern S3KGlobalVariables *globals;
extern S3KExtensionsGlobalVariables* modGlobals;
extern S3KExtensionsConfig config;

// Objects
#include "Objects/DebugMenu.hpp"
#include "Objects/DrawHelpers.hpp"
#include "Objects/HUD.hpp"
#include "Objects/Mission.hpp"
#include "Objects/OptionsMenu.hpp"
#include "Objects/PauseMenu.hpp"
#include "Objects/Player.hpp"
#include "Objects/S3K_SS_Player.hpp"
#include "Objects/S3K_SaveMenu.hpp"
#include "Objects/S3K_SaveSlot.hpp"
#include "Objects/SignPost2.hpp"
