#include "types.h"
#include "game/row_lock_mark_task.h"
#include "game/screen_other_map.h"

s32 HwDiv32(s32 numerator, s32 denominator);
int Math_Ease(int curve, int from, int to, int elapsed, int duration);
void Sprite_Queue(int list, int x, int y, void *data, int attr, int tile);
void Oam_SetAffineParams(int index, s16 pa, s16 pb, s16 pc, s16 pd);

extern const s16 gSineTable[];
extern u8 data_027c0060;

#define DIVISOR(scale) ((scale) ? (scale) : 2)

#define POP_IN_FRAMES 4
#define MARK_TILE     0x124

/* Task script step that shows a row's lock mark after the row is locked:
 * over POP_IN_FRAMES frames the mark shrinks from twice its size to its
 * normal size, drawn as a double-size affine sprite. After that it is drawn
 * as a plain sprite until RowLockMark_SquashOut's step (the script holds
 * here until RowLockMark_Remove resumes it). */
void RowLockMark_PopIn(RowLockMarkTask *task)
{
    Unk_027c0280 *savedGfx = data_027c0280;
    Unk_02080440 *savedState;

    Screen_SetCurrentGfx(data_027c0288.unk_04);
    savedState = data_027c027c;
    Screen_SetCurrent(data_027c0288.unk_04);
    if (task->frame >= POP_IN_FRAMES) {
        Sprite_Queue(1, task->x & 0x1ff, task->y & 0xff, &data_027c0060, 0, MARK_TILE);
    } else {
        int scale = Math_Ease(0, 512, 256, task->frame, POP_IN_FRAMES);
        int d3 = DIVISOR(scale);
        int d2 = DIVISOR(scale);
        int d1 = DIVISOR(scale);
        int d0 = DIVISOR(scale);

        Oam_SetAffineParams(task->affineIndex, HwDiv32(gSineTable[64] * 16, d0),
                            HwDiv32(-gSineTable[0] * 16, d1), HwDiv32(gSineTable[0] * 16, d2),
                            HwDiv32(gSineTable[64] * 16, d3));
        /* Affine double-size sprite (0x300): the 32x16 box grows to 64x32, so x and y move back by 16 and 8. */
        Sprite_Queue(1, ((task->x - 16) & 0x1ff) | (task->affineIndex << 9), ((task->y - 8) & 0xff) | 0x300,
                     &data_027c0060, 0, MARK_TILE);
        task->frame++;
    }
    Screen_SetCurrent(savedState);
    Screen_SetCurrentGfx(savedGfx);
}
