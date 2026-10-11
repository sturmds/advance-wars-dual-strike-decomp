#include "types.h"
#include "sdk/gx.h"

/* Sets the color, alpha, polygon ID and fog flag that clear the frame
 * buffer, and the depth that clears the depth buffer. */
void G3X_SetClearColor(int color, int alpha, int depth, int polygonId, BOOL fog)
{
    u32 clearColor = color | (alpha << 16) | (polygonId << 24);

    if (fog) {
        clearColor |= CLEAR_COLOR_FOG;
    }
    *REG_CLEAR_COLOR = clearColor;
    *REG_CLEAR_DEPTH = depth;
}
