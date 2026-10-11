#include "types.h"
#include "game/Unk_02080440.h"

extern Unk_02080440 *data_027c027c;
extern int data_027c0284;
extern Unk_02080440_48 data_02183b08;

int MainLoop_GetFrameCount(void);
/* These take the frame count; the matched ones ignore it. */
void ScreenConfig_RunFrameHook(int frame);
void PowerMeter_UpdateFlash(int frame);
void ScreenConfig_RunCycleHook(int frame);
void PowerPanel_AnimatePaletteColour(int frame);
void MapAnim_QueueSecondFramePart(int frame);
void MapAnim_QueueFramePart(int frame);
void MapAnim_QueueFramePartMapF8(int frame);

#define GAME (data_027c027c->unk_48[data_027c0284])

/* Per-frame map animation updates, each given the frame count. On the first
 * screen's state, map 0xf8 queues its animation through MapAnim_QueueFramePartMapF8
 * instead of MapAnim_QueueFramePart. */
void MapScreen_UpdateAnimations(void)
{
    ScreenConfig_RunFrameHook(MainLoop_GetFrameCount());
    PowerMeter_UpdateFlash(MainLoop_GetFrameCount());
    ScreenConfig_RunCycleHook(MainLoop_GetFrameCount());
    PowerPanel_AnimatePaletteColour(MainLoop_GetFrameCount());
    MapAnim_QueueSecondFramePart(MainLoop_GetFrameCount());
    if (GAME == &data_02183b08 && GAME->unk_1c4->mapId == 0xf8) {
        MapAnim_QueueFramePartMapF8(MainLoop_GetFrameCount());
    } else {
        MapAnim_QueueFramePart(MainLoop_GetFrameCount());
    }
}
