#include "types.h"
#include "game/day_banner_task.h"
#include "game/screen_other_map.h"

s32 HwDiv32(s32 numerator, s32 denominator);
int Math_Ease(int curve, int from, int to, int elapsed, int duration);
void Sprite_Queue(int list, int x, int y, void *data, int attr, int tile);
void Sprite_QueueAt(int list, int x, int y, void *data, int attr, int tile);
void Oam_SetAffineParams(int index, s16 pa, s16 pb, s16 pc, s16 pd);
void Task_ResumeScript(DayBannerTask *task);
void DayBanner_RefreshLayer(void);

extern const s16 gSineTable[];
extern u8 data_027c0048[];

#define DIVISOR(scale) ((scale) ? (scale) : 2)

#define WORD_SPRITES   3               /* sprites before the digits */
#define SPRITE_COUNT(task) ((task)->digitCount + WORD_SPRITES)
#define STAGGER_FRAMES 6               /* between the starts of two sprites */
#define SHRINK_FRAMES  16
#define TOTAL_FRAMES(task) (SPRITE_COUNT(task) * STAGGER_FRAMES + SHRINK_FRAMES)
#define BANNER_Y       80
#define WORD_TILE      404             /* first tile of the word sprites, 16 tiles each */
#define DIGIT_TILE     452             /* tile of digit 0, 16 tiles per digit */
#define BANNER_PALETTE (3 << 12)

/* Task script step that closes the day banner. Starting with the last digit,
 * every STAGGER_FRAMES frames one more sprite shrinks horizontally to nothing
 * over SHRINK_FRAMES frames, using OBJ affine matrix i + 1. Meanwhile layer 0
 * scrolls away and its alpha blend fades (EVA 12 to 0). When the last sprite
 * is gone the frame counter resets and the script resumes. */
void DayBanner_CloseUpdate(DayBannerTask *task)
{
    int i;
    int eva;
    int bg;

    task->frame++;
    eva = Math_Ease(1, 12, 0, task->frame, TOTAL_FRAMES(task));
    data_027c0280->blendMode = BLEND_MODE_ALPHA;
    data_027c0280->blendEva = eva;
    data_027c0280->blendEvb = 16 - eva;
    data_027c0280->blendEvy = 0;
    /* Needed to match: the BG index is read before the Math_Ease call. */
    bg = data_027c0280->layer0Bg;
    data_027c0280->bgScrollX[bg] = Math_Ease(1, 0, 128, task->frame, TOTAL_FRAMES(task));
    bg = data_027c0280->layer0Bg;
    data_027c0280->bgScrollY[bg] = Math_Ease(1, 0, 96, task->frame, TOTAL_FRAMES(task)) + 32;
    DayBanner_RefreshLayer();

    for (i = 0; i < SPRITE_COUNT(task); i++) {
        int start;

        /* Needed to match: the original tests the loop condition again here. */
        if (i >= SPRITE_COUNT(task)) {
            continue;
        }
        start = (SPRITE_COUNT(task) - 1 - i) * STAGGER_FRAMES;

        if (task->frame <= start) {
            if (i < WORD_SPRITES) {
                Sprite_QueueAt(0, (task->wordX + i * 32) & 0x1ff, BANNER_Y, data_027c0048, BANNER_PALETTE | (WORD_TILE + i * 16), 0);
            } else {
                Sprite_QueueAt(0, (task->digitsX + (i - WORD_SPRITES) * 28) & 0x1ff, BANNER_Y, data_027c0048,
                               BANNER_PALETTE | (((task->digits >> ((i - WORD_SPRITES) * 4)) & 15) * 16 + DIGIT_TILE), 0);
            }
        } else if (task->frame <= start + SHRINK_FRAMES) {
            int scaleX = Math_Ease(0, 256, 2, task->frame - start, SHRINK_FRAMES);
            int scaleY = Math_Ease(0, 256, 256, task->frame - (SPRITE_COUNT(task) - 1 - i) * STAGGER_FRAMES, SHRINK_FRAMES);
            int d3 = DIVISOR(scaleY);
            int d2 = DIVISOR(scaleX);
            int d1 = DIVISOR(scaleY);
            int d0 = DIVISOR(scaleX);

            Oam_SetAffineParams(i + 1, HwDiv32(gSineTable[64] * 16, d0), HwDiv32(-gSineTable[0] * 16, d1),
                                HwDiv32(gSineTable[0] * 16, d2), HwDiv32(gSineTable[64] * 16, d3));
            /* Affine double-size sprites (0x300): the 32-pixel box grows to 64, so x and y move back by 16. */
            if (i < WORD_SPRITES) {
                Sprite_Queue(0, ((task->wordX + i * 32 - 16) & 0x1ff) | ((i + 1) << 9), ((BANNER_Y - 16) & 0xff) | 0x300,
                             data_027c0048, BANNER_PALETTE | (WORD_TILE + i * 16), 0);
            } else {
                Sprite_Queue(0, ((task->digitsX + (i - WORD_SPRITES) * 28 - 16) & 0x1ff) | ((i + 1) << 9),
                             ((BANNER_Y - 16) & 0xff) | 0x300, data_027c0048,
                             BANNER_PALETTE | (((task->digits >> ((i - WORD_SPRITES) * 4)) & 15) * 16 + DIGIT_TILE), 0);
            }
        }
    }

    if (task->frame == TOTAL_FRAMES(task)) {
        task->frame = 0;
        Task_ResumeScript(task);
    }
}
