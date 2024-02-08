#ifndef S3K_H
#define S3K_H
#include <Game.h>

typedef struct {
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
} GlobalS3KVariables;

#endif // !ifdef S3K_H