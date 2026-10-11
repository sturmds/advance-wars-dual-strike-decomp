#include "types.h"
#include "game/Unk_ov003_0235dd38.h"

/* Needed to match: const, so the sine reads move out of the loop. */
extern const s16 gSineTable[];

s32 HwDiv32(s32 numerator, s32 denominator);
int Math_Ease(int curve, int from, int to, int elapsed, int duration);
void Sprite_QueueCoPortrait(int x, int y, int arg2, int arg3, int list, BOOL flip);
void Oam_SetAffineParams(int index, s16 pa, s16 pb, s16 pc, s16 pd);
void BreakText_QueueLetterSized(u8 letter, u16 x, int y, int arg3, int list);
s16 BreakText_GetLetterX(u8 letter);
void Task_ResumeScript(Unk_ov003_0235dd38 *task);

/* Divisor for one matrix entry: the eased scale, or 2 if it is 0. */
#define DIVISOR(scale) ((scale) ? (scale) : 2)

/* Step of the CO Power cut-in, the same as Ov003_StepLettersGrow apart from
 * the sprite arguments: over 3 * count + 8 frames the sprite at
 * Sprite_QueueCoPortrait slides 240 pixels left, and each letter of the power name
 * (count from CoPowerScreen_SetUp) grows to double size through affine
 * matrix i + 1, starting 3 frames after the previous one, then is no longer
 * drawn. */
void CoPowerScreen_GrowOutName(Unk_ov003_0235dd38 *task)
{
    int i;
    int scale;

    task->frame++;
    task->slideX = Math_Ease(1, 0, -240, task->frame, task->count * 3 + 8);
    Sprite_QueueCoPortrait((task->slideX + 184) & 0x1ff, 160, 0, 16, 2, 0);
    for (i = 0; i < task->count; i++) {
        if (task->frame < i * 3) {
            BreakText_QueueLetterSized(i, BreakText_GetLetterX(i) & 0x1ff, 80, 0, 1);
        } else if (task->frame >= i * 3 && task->frame < i * 3 + 8) {
            scale = Math_Ease(1, 256, 512, task->frame - i * 3, 8);
            {
                int d3 = DIVISOR(scale);
                int d2 = DIVISOR(scale);
                int d1 = DIVISOR(scale);
                int d0 = DIVISOR(scale);
                int cosValue = gSineTable[64] * 16;
                int sinPos = gSineTable[0] * 16;
                int sinNeg = -gSineTable[0] * 16;
                Oam_SetAffineParams(i + 1, HwDiv32(cosValue, d0), HwDiv32(sinNeg, d1),
                              HwDiv32(sinPos, d2), HwDiv32(cosValue, d3));
            }
            BreakText_QueueLetterSized(i, (BreakText_GetLetterX(i) & 0x1ff) | ((i + 1) << 9), 0x350, 0, 1);
        }
    }
    if (task->frame == task->count * 3 + 8) {
        task->frame = 0;
        Task_ResumeScript(task);
    }
}
