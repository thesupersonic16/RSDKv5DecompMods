#include "S3KExtensions.hpp"

using namespace RSDK;
using namespace GameLogic;
using namespace Helpers;

void (*EngineNotifyCallback)(int32 callbackID, int32 param1, int32 param2, int32 param3);

extern "C" void UnloadMod() {
    // I hope mods unload in order
    if (EngineNotifyCallback)
        RSDKTable->NotifyCallback = EngineNotifyCallback;
}

void OnNotifyCallback(int32 callback, int32 param1, int32 param2, int32 param3)
{
    switch (callback) {
    case NOTIFY_DEATH_EVENT: {
        for (auto entity : GameObject::GetEntities<Mission>(FOR_ACTIVE_ENTITIES))
            entity->SetPlayerDeath();
        return;
    }
    case NOTIFY_TOUCH_SIGNPOST: {
        for (auto entity : GameObject::GetEntities<Mission>(FOR_ACTIVE_ENTITIES))
            entity->SetCompleted();
        return;
    }
    case NOTIFY_KILL_ENEMY: {
        switch (param1) {
        case 0:
        case 5:
            modGlobals->missionProgress[0]++;
            break;
        case 4:
            modGlobals->missionProgress[0] += param2;
            break;
        default:
            Dev::Print(Dev::PRINT_POPUP, "%d, %d, %d, %d", callback, param1, param2, param3);
            modGlobals->missionProgress[0]++;
            break;
        }
        return;
    }
    default:
        break;
    }
    Dev::Print(Dev::PRINT_POPUP, "%d, %d, %d, %d", callback, param1, param2, param3);

    if (EngineNotifyCallback)
        EngineNotifyCallback(callback, param1, param2, param3);
}

void OnStageLoad(void* data) {
    // Don't allow mission mode outside of missions
    if (!modGlobals->startMission && globals->playMode == BOOT_PLAYMODE_MISSION) {
        globals->playMode = 0;
        modGlobals->missionData = nullptr;
    }

    if (config.missionHandlerEnable && modGlobals->startMission) {
        // Spawn Mission object on current loading scene
        if (Mission::sVars == nullptr) {
            Dev::Print(Dev::PRINT_NORMAL, "Mission::sVars is null!");
            return;
        }
        GameObject::Create<Mission>(NULL, 0, 0);
    }
}