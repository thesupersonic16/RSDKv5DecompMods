#pragma once

#if _WINDOWS
#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
// Windows Header Files
#include <windows.h>
void errorMessage(char* text, char* caption)
{
    MessageBoxA(NULL, text, caption, MB_ICONERROR);
}
#else
#include <cstdio>
void errorMessage(const char* text, const char* caption)
{
    printf("\e[1;91merror:\e[0m [%s] %s\n", caption, text);
}
#endif