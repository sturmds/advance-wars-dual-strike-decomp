#include "types.h"
#include "game/campaign_recap.h"

s32 HwDiv32(s32 numer, s32 denom);
s32 HwMod32(s32 numerator, s32 denominator);
void Sprite_Queue(int list, int x, int y, void* data, int attr, int tile);

extern u8 data_027c0040[];

#define NO_DIGIT (-1)

#define QUEUE_DIGIT(digitX, digit)                                        \
  Sprite_Queue(2, (digitX) & 0x1ff, (y & 0xff) | (semiTransparent << 10), \
               data_027c0040, palette << 12, 0x2e4 + (digit) * 4)

/* Draws a number from 0 to 9999 as four digit sprites 14 pixels apart, the
 * hundreds digit at x. Leading zeros are left out; the ones digit always
 * shows. */
void CampaignRecap_DrawNumber(CampaignRecap* work, int value, int x, int y,
                              int semiTransparent, int palette) {
  int thousands = HwDiv32(value, 1000);
  int hundreds;
  int tens;
  int ones;

  value = HwMod32(value, 1000);
  hundreds = HwDiv32(value, 100);
  ones = HwMod32(value, 100);
  tens = HwDiv32(ones, 10);
  ones = HwMod32(ones, 10);

  if (thousands == 0) {
    thousands = NO_DIGIT;
    if (hundreds == 0) {
      hundreds = NO_DIGIT;
      if (tens == 0) {
        tens = NO_DIGIT;
      }
    }
  }

  if (thousands != NO_DIGIT) {
    QUEUE_DIGIT(x - 14, thousands);
  }
  if (hundreds != NO_DIGIT) {
    QUEUE_DIGIT(x, hundreds);
  }
  if (tens != NO_DIGIT) {
    QUEUE_DIGIT(x + 14, tens);
  }
  if (ones != NO_DIGIT) {
    QUEUE_DIGIT(x + 28, ones);
  }
}
