#include "types.h"
#include "game/link_marker_task.h"
#include "game/screen_other_map.h"

s32 HwDiv32(s32 numer, s32 denom);
int Math_Ease(int curve, int from, int to, int elapsed, int duration);
void Oam_SetAffineParams(int index, s16 pa, s16 pb, s16 pc, s16 pd);
void Sprite_Queue(int list, int x, int y, void *data, int attr, int tile);

/* Needed to match: const lets mwcc load the sine and cosine once. */
extern const s16 gSineTable[];
extern u8 data_027c0060[];

/* Divisor for one matrix entry: the eased scale, or 2 if it is 0. */
#define DIVISOR(scale) ((scale) ? (scale) : 2)

/* Task step of a link menu marker, on the screen of data_027c0288.unk_00: for
 * the first 4 frames draws the marker through affine set affineIndex, growing from
 * half to full size; after that draws it plain. */
void LinkMarkerTask_GrowIn(LinkMarkerTask *task)
{
    Unk_027c0280 *savedGfx;
    Unk_02080440 *savedState;
    int scale;

    savedGfx = data_027c0280;
    Screen_SetCurrentGfx(data_027c0288.unk_00);
    savedState = data_027c027c;
    Screen_SetCurrent(data_027c0288.unk_00);
    if (task->frame >= 4) {
        Sprite_Queue(0, task->x & 0x1ff, task->y & 0xff, data_027c0060, 0, 52);
    } else {
        scale = Math_Ease(0, 0x200, 0x100, task->frame, 4);
        {
            int pdDiv = DIVISOR(scale);
            int pcDiv = DIVISOR(scale);
            int pbDiv = DIVISOR(scale);
            int paDiv = DIVISOR(scale);

            Oam_SetAffineParams(task->affineIndex, HwDiv32(gSineTable[64] * 16, paDiv),
                                HwDiv32(-gSineTable[0] * 16, pbDiv),
                                HwDiv32(gSineTable[0] * 16, pcDiv),
                                HwDiv32(gSineTable[64] * 16, pdDiv));
        }
        Sprite_Queue(0, ((task->x - 16) & 0x1ff) | (task->affineIndex << 9), ((task->y - 8) & 0xff) | 0x300,
                     data_027c0060, 0, 52);
        task->frame++;
    }
    Screen_SetCurrent(savedState);
    Screen_SetCurrentGfx(savedGfx);
}
