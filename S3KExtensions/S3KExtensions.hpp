#pragma once
#include "GameAPI/Game.hpp"

// Mod
struct S3KExtensionsConfig {
    bool pauseMenuEnable;
    bool optionsMenuEnable;
};

// Variables
struct S3KGlobalVariables {
    uint8 gameMode;
    uint32 playerID;
    uint8 gap8[0xC2CAC];
    uint32 medalMods;
    uint8 gapC2CB8[0x400748];
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
    bool32 playMode;
    int32 unknown4C3514;
    int32 unknown4C3518;
    int32 unknown4C351C;
    int32 unknown4C3520;
    int32 unknown4C3524;
    bool32 useCoins;
    uint8 gap4C352C[0x60];
    bool32 waitSSRetry;
};

extern S3KGlobalVariables *globals;
extern S3KExtensionsConfig config;

// Helpers
#include "Helpers.hpp"

// Objects
#include "Objects/OptionsMenu.hpp"
#include "Objects/PauseMenu.hpp"
#include "Objects/Player.hpp"
#include "Objects/S3K_SS_Player.hpp"
#include "Objects/S3K_SaveMenu.hpp"
#include "Objects/S3K_SaveSlot.hpp"
