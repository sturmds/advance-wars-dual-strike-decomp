#include "types.h"
#include "game/Unk_02080440.h"

extern Unk_02080440 *data_027c027c;
extern int data_027c0284;
extern Unk_02080440_48 data_02183b08;

int MainLoop_GetFrameCount(void);
/* These take the frame count; the matched ones ignore it. */
void func_020f76dc(int frame);
void PowerMeter_UpdateFlash(int frame);
void func_020f63b0(int frame);
void func_020e1714(int frame);
void MapAnim_QueueSecondFramePart(int frame);
void MapAnim_QueueFramePart(int frame);
void func_020fac6c(int frame);

#define GAME (data_027c027c->unk_48[data_027c0284])

/* Per-frame map animation updates, each given the frame count. On the first
 * screen's state, map 0xf8 queues its animation through func_020fac6c
 * instead of MapAnim_QueueFramePart. */
void MapScreen_UpdateAnimations(void)
{
    func_020f76dc(MainLoop_GetFrameCount());
    PowerMeter_UpdateFlash(MainLoop_GetFrameCount());
    func_020f63b0(MainLoop_GetFrameCount());
    func_020e1714(MainLoop_GetFrameCount());
    MapAnim_QueueSecondFramePart(MainLoop_GetFrameCount());
    if (GAME == &data_02183b08 && GAME->unk_1c4->mapId == 0xf8) {
        func_020fac6c(MainLoop_GetFrameCount());
    } else {
        MapAnim_QueueFramePart(MainLoop_GetFrameCount());
    }
}
