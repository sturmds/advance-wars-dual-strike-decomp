#include "types.h"
#include "game/task.h"
#include "game/screen_other_map.h"

s32 HwDiv32(s32 numer, s32 denom);
int Math_Ease(int curve, int from, int to, int elapsed, int duration);
void Sprite_Queue(int list, int x, int y, void *data, int attr, int tile);
void Oam_SetAffineParams(int index, s16 pa, s16 pb, s16 pc, s16 pd);
void Task_ResumeScript(Task *task);
void func_020772d8(int x);
void func_020773a0(int x);

extern u8 data_027c0060;
extern s16 gSineTable[];

/* The frame counter shares its slot with Task.unk_4c. */
#define FRAME(task) (*(s16 *)&(task)->unk_4c)

/* Divisor for one matrix entry: the eased scale, or 2 if it is 0. */
#define DIVISOR(scale) ((scale) ? (scale) : 2)

/* Task update: over 4 frames, draws one sprite on the sub screen through
 * affine matrix 1, scaled from 2x down to 1x, then resumes the task's
 * script. func_020773a0 and func_020772d8 draw the rest of the scene.
 * gSineTable has 256 steps per turn (index 64 = cosine of 0). */
void SubScreenSprite_ZoomInUpdate(Task *task)
{
    Unk_027c0280 *saved0280;
    Unk_02080440 *saved027c;
    int scale;

    FRAME(task)++;
    func_020773a0(0);
    func_020772d8(0);

    saved0280 = data_027c0280;
    Screen_SetCurrentGfx(data_027c0288.unk_04);
    saved027c = data_027c027c;
    Screen_SetCurrent(data_027c0288.unk_04);
    scale = Math_Ease(0, 0x200, 0x100, FRAME(task), 4);
    /* Needed to match: the divisor is evaluated once per entry, before the
     * table reads. */
    {
        int d3 = DIVISOR(scale);
        int d2 = DIVISOR(scale);
        int d1 = DIVISOR(scale);
        int d0 = DIVISOR(scale);
        int cosValue = gSineTable[64] * 16;
        int sinValue = gSineTable[0];

        Oam_SetAffineParams(1, HwDiv32(cosValue, d0), HwDiv32(-sinValue * 16, d1),
                            HwDiv32(sinValue * 16, d2), HwDiv32(cosValue, d3));
    }
    /* x 206 with matrix 1; y 136, affine and double size. */
    Sprite_Queue(3, 206 | (1 << 9), 136 | 0x300, &data_027c0060, 0x70b8, 0);
    Screen_SetCurrent(saved027c);
    Screen_SetCurrentGfx(saved0280);

    if (FRAME(task) == 4) {
        FRAME(task) = 0;
        Task_ResumeScript(task);
    }
}
