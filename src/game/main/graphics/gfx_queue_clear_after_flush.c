#include "types.h"
#include "game/gfx_queue.h"

/* Called once the queue is copied to VRAM: resets the counters and empties
 * every entry except kind 3 and 4, which wait for GfxQueue_FreeOwnedCopies. */
void GfxQueue_ClearAfterFlush(void)
{
    int i;

    data_027c051c = 0;
    data_027c0518 = 0;
    for (i = 0; i < GFX_QUEUE_LENGTH; i++) {
        if (!GFX_QUEUE_OWNS_SRC(data_027c0520[i].kind)) {
            data_027c0520[i].src = NULL;
            data_027c0520[i].dst = NULL;
            data_027c0520[i].size = 0;
            data_027c0520[i].kind = 0;
        }
    }
}
