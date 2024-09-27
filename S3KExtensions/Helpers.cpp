#include "S3KExtensions.hpp"

namespace Helpers {
    bool32 CheckTouch(int32 x, int32 y, int32 w, int32 h)
    {
        for (int32 t = 0; t < touchInfo->count; ++t) {
            int32 tx = (int32)(touchInfo->x[t] * screenInfo->size.x);
            int32 ty = (int32)(touchInfo->y[t] * screenInfo->size.y);

            if (touchInfo->down[t] && tx >= x && ty >= y && tx <= x + w && ty <= y + h)
                return true;
        }

        return false;
    }
} //namespace Helpers