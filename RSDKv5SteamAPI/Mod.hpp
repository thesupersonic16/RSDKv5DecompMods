#pragma once
extern "C"
{
#include <Game.h>
}
#include <Helpers.h>

#define DLC_COUNT 8

#ifdef _AMD64_
#define DEFAULT_CALL_TYPE __fastcall
#else
#ifdef _X86_
#define DEFAULT_CALL_TYPE __cdecl
#endif
#endif
#define DisplayError(message) MessageBoxA(NULL, message, "RSDKv5SteamAPI Error", MB_ICONERROR);

extern void* ModHandle;

void CheckDLCs();