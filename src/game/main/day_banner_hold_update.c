#include "types.h"
#include "game/day_banner_task.h"

void Sprite_QueueAt(int list, int x, int y, void* data, int attr, int tile);
void Task_ResumeScript(DayBannerTask* task);

extern u8 data_027c0048[];

#define WORD_SPRITES 3 /* sprites before the digits */
#define SPRITE_COUNT(task) ((task)->digitCount + WORD_SPRITES)
#define HOLD_FRAMES 32
#define BANNER_Y 80
#define WORD_TILE 404  /* first tile of the word sprites, 16 tiles each */
#define DIGIT_TILE 452 /* tile of digit 0, 16 tiles per digit */
#define BANNER_PALETTE (3 << 12)

/* Task script step between DayBanner_OpenUpdate and DayBanner_CloseUpdate:
 * draws the word and digit sprites at their normal size for HOLD_FRAMES
 * frames, then resets the frame counter and resumes the script. */
void DayBanner_HoldUpdate(DayBannerTask* task) {
  int i;

  task->frame++;
  for (i = 0; i < SPRITE_COUNT(task); i++) {
    /* Needed to match: the original tests the loop condition again here. */
    if (i >= SPRITE_COUNT(task)) {
      continue;
    }
    if (i < WORD_SPRITES) {
      Sprite_QueueAt(0, (task->wordX + i * 32) & 0x1ff, BANNER_Y, data_027c0048,
                     BANNER_PALETTE | (WORD_TILE + i * 16), 0);
    } else {
      Sprite_QueueAt(
          0, (task->digitsX + (i - WORD_SPRITES) * 28) & 0x1ff, BANNER_Y,
          data_027c0048,
          BANNER_PALETTE |
              (((task->digits >> ((i - WORD_SPRITES) * 4)) & 15) * 16 +
               DIGIT_TILE),
          0);
    }
  }

  if (task->frame == HOLD_FRAMES) {
    task->frame = 0;
    Task_ResumeScript(task);
  }
}
