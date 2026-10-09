#include "types.h"

s32 HwDiv32(s32 numer, s32 denom);
int Math_Ease(int curve, int from, int to, int elapsed, int duration);
void Sprite_Queue(int list, int x, int y, void *data, int attr, int tile);
void Sprite_QueueAt(int list, int x, int y, void *data, int attr, int tile);
void func_02003250(int index, s16 pa, s16 pb, s16 pc, s16 pd);

extern u8 data_027c0028;
extern s16 data_021389e8[];

/* Queues one digit sprite at (x, y). Frames 0 to 8 play a pop-in: the sprite
 * is drawn through affine matrix `matrixIndex`, scaled by an ease from 2x
 * down to 1x. Any other frame draws it plain. WIP: 82%, the original keeps
 * the constant 5 in a register. */
void func_020342ac(int x, int y, int digit, int matrixIndex, int frame)
{
    int scale;
    int list;
    int sx, sy;

    list = 5;
    if (frame < 0 || frame > 8) {
        Sprite_QueueAt(6, x & 0x1ff, y & 0xff, &data_027c0028, 0xc000, 0x330 + digit * 2);
        return;
    }
    scale = Math_Ease(4, 0x200, 0x100, frame, 8);
    sy = data_021389e8[64] * 16;
    sx = data_021389e8[0];
    func_02003250(matrixIndex, HwDiv32(sy, scale ? scale : 2), HwDiv32(-sx * 16, scale ? scale : 2),
                  HwDiv32(sx * 16, scale ? scale : 2), HwDiv32(sy, scale ? scale : 2));
    Sprite_Queue(list - ((scale - 0x100) >> 6), ((x - 4) & 0x1ff) | (matrixIndex << 9), ((y - 4) & 0xff) | 0x300,
                 &data_027c0028, 0xc000, 0x330 + digit * 2);
}
