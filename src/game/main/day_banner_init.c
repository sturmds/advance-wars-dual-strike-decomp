#include "types.h"
#include "game/day_banner_task.h"
#include "game/screen_other_map.h"

s32 HwDiv32(s32 numerator, s32 denominator);
s32 HwMod32(s32 numerator, s32 denominator);
void DayBanner_RefreshLayer(void);

#define WORD_WIDTH 104 /* the three word sprites */
#define DIGIT_WIDTH 28

/* Task script step that sets up the day banner: splits the current day into
 * decimal digits without leading zeros, starts layer 0 off screen and centres
 * the word and the number on the 256-pixel screen. */
void DayBanner_Init(DayBannerTask* task) {
  u8 hundreds =
      HwMod32(HwDiv32(data_027c027c->unk_48[data_027c0284]->day, 100), 10);
  u8 tens = HwMod32(HwDiv32(data_027c027c->unk_48[data_027c0284]->day, 10), 10);
  u8 ones = HwMod32(data_027c027c->unk_48[data_027c0284]->day, 10);

  if (hundreds == 0) {
    if (tens == 0) {
      task->digits = ones;
      task->digitCount = 1;
    } else {
      task->digits = tens | (ones << 4);
      task->digitCount = 2;
    }
  } else {
    task->digits = hundreds | (tens << 4) | (ones << 8);
    task->digitCount = 3;
  }

  data_027c0280->bgScrollX[data_027c0280->layer0Bg] = -128;
  data_027c0280->bgScrollY[data_027c0280->layer0Bg] = -64;
  DayBanner_RefreshLayer();

  task->wordX = 128 - ((task->digitCount * DIGIT_WIDTH + WORD_WIDTH) >> 1);
  task->digitsX =
      128 + WORD_WIDTH - ((task->digitCount * DIGIT_WIDTH + WORD_WIDTH) >> 1);
  task->frame = 0;
}
