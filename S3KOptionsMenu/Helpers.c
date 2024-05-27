#include "Helpers.h"

bool32 CheckTouch(int32 x, int32 y, int32 w, int32 h)
{
    for (int32 t = 0; t < TouchInfo->count; ++t) {
        int32 tx = (int32)(TouchInfo->x[t] * ScreenInfo->size.x);
        int32 ty = (int32)(TouchInfo->y[t] * ScreenInfo->size.y);

        if (TouchInfo->down[t] && tx >= x && ty >= y && tx <= x + w && ty <= y + h)
            return true;
    }

    return false;
}