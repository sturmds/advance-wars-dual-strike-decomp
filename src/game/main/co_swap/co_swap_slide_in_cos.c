#include "types.h"
#include "game/co_swap.h"

int Math_Ease(int curve, int from, int to, int elapsed, int duration);
s32 HwDiv32(s32 numerator, s32 denominator);
void Oam_SetAffineParams(int index, s16 pa, s16 pb, s16 pc, s16 pd);
void Sprite_QueueCoPortrait(int x, int y, int arg2, int arg3, int list, BOOL flip);
s16 BreakText_GetScreenX(u8 slot);
void BreakText_QueueLetter(u8 slot, u16 x, int y, int attr, int arg);
void Task_ResumeScript(Task *task);

/* Looks like a 256-entry sine table: index 0 is the sine of 0, index 64 its
 * cosine, so the matrices below have no rotation. const lets the compiler
 * read it once before the loop, as the original does. */
extern const s16 gSineTable[];

#define LETTER_DELAY  4                /* frames between two letters starting */
#define LETTER_FRAMES 24               /* frames one letter takes to grow */
#define OBJ_AFFINE    0x100            /* OAM attribute 0: affine sprite */
#define OBJ_MATRIX(n) ((n) << 9)       /* OAM attribute 1: affine matrix */

/* Script step of the CO swap cut-in (UnitAction_SwapCo). The front and back
 * CO pictures slide in from 256 pixels out to their resting x, while the
 * task->unk_58 title letters grow in height, one starting every 4 frames
 * (letter i uses affine matrix i + 1). Resumes the script when the last
 * letter is done. */
void CoSwap_SlideInCos(Task *task)
{
    int i;
    int height;
    /* Needed to match: the divisors are worked out once per matrix entry. */
    int pdDivisor;
    int pbDivisor;

    CO_SWAP_FRAME(task)++;
    CO_SWAP_OFFSET(task) = Math_Ease(4, 256, 0, CO_SWAP_FRAME(task), task->unk_58 * LETTER_DELAY + LETTER_FRAMES);
    Sprite_QueueCoPortrait((CO_SWAP_FRONT_X + CO_SWAP_OFFSET(task)) & 0x1ff, CO_SWAP_CO_Y, 0, 16, 2, FALSE);
    Sprite_QueueCoPortrait((CO_SWAP_BACK_X - CO_SWAP_OFFSET(task)) & 0x1ff, CO_SWAP_CO_Y, 0x1000, 0x1c0, 3, FALSE);
    for (i = 0; i < task->unk_58; i++) {
        if (CO_SWAP_FRAME(task) < i * LETTER_DELAY) {
            continue;
        }
        if (CO_SWAP_FRAME(task) < i * LETTER_DELAY + LETTER_FRAMES) {
            height = Math_Ease(4, 16, 256, CO_SWAP_FRAME(task) - i * LETTER_DELAY, LETTER_FRAMES);
            pdDivisor = height ? height : 2;
            pbDivisor = height ? height : 2;
            Oam_SetAffineParams(i + 1, HwDiv32(gSineTable[64] * 16, 256),
                                HwDiv32(-gSineTable[0] * 16, pbDivisor),
                                HwDiv32(gSineTable[0] * 16, 256),
                                HwDiv32(gSineTable[64] * 16, pdDivisor));
            BreakText_QueueLetter(i, (BreakText_GetScreenX(i) & 0x1ff) | OBJ_MATRIX(i + 1),
                          CO_SWAP_TITLE_Y | OBJ_AFFINE, 0, 1);
        } else {
            BreakText_QueueLetter(i, BreakText_GetScreenX(i) & 0x1ff, CO_SWAP_TITLE_Y, 0, 1);
        }
    }
    if (CO_SWAP_FRAME(task) == task->unk_58 * LETTER_DELAY + LETTER_FRAMES) {
        CO_SWAP_FRAME(task) = 0;
        Task_ResumeScript(task);
    }
}
