#pragma once
#include "GameAPI/Game.hpp"

extern void (*EngineNotifyCallback)(int32 callbackID, int32 param1, int32 param2, int32 param3);

extern "C" DLLExport void UnloadMod();
void OnNotifyCallback(int32 callback, int32 param1, int32 param2, int32 param3);
void OnStageLoad(void* data);