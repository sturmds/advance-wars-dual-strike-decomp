#include "types.h"
#include "game/main_loop.h"
#include "game/screen_other_map.h"

extern MainLoop gMainLoop;

/* Sets the per-frame callback of the current screen (NULL clears it). */
void MainLoop_SetFrameCallback(void (*callback)(void))
{
    if (data_027c027c == data_027c0288.unk_00) {
        gMainLoop.screenCallbacks[0] = callback;
    } else {
        gMainLoop.screenCallbacks[1] = callback;
    }
}
