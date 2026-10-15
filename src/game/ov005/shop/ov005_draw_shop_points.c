#include "types.h"
#include "game/Unk_02080440.h"
#include "game/Unk_02088d04.h"
#include "game/icon.h"
#include "game/Unk_ov005_0236bbb0.h"
#include "game/math.h"
#include "game/screen_other_map.h"

/* Leading zeros become -1 (not drawn) until the first nonzero digit. */
#define NEXT_DIGIT(digit, unit) \
  digit = HwDiv32(rest, unit);  \
  rest -= digit * (unit);       \
  if (!seen && digit == 0) {    \
    digit = -1;                 \
  } else {                      \
    seen = TRUE;                \
  }

/* Draws icon 212 at the top left and work->points (up to 8 digits, digit
 * icons 140-149) right-aligned at the top right of the main screen. */
void Ov005_DrawShopPoints(Unk_ov005_0236bbb0* work) {
  int rest = work->points;
  BOOL seen = FALSE;
  int d10000000, d1000000, d100000, d10000, d1000, d100, d10;
  Unk_027c0280* saved0280;
  Unk_02080440* saved027c;

  NEXT_DIGIT(d10000000, 10000000);
  NEXT_DIGIT(d1000000, 1000000);
  NEXT_DIGIT(d100000, 100000);
  NEXT_DIGIT(d10000, 10000);
  NEXT_DIGIT(d1000, 1000);
  NEXT_DIGIT(d100, 100);
  d10 = HwDiv32(rest, 10);
  rest -= d10 * 10;
  if (!seen && d10 == 0) {
    d10 = -1;
  }

  saved0280 = data_027c0280;
  Screen_SetCurrentGfx(data_027c0288.unk_00);
  saved027c = data_027c027c;
  Screen_SetCurrent(data_027c0288.unk_00);
  Icon_Draw(212, 16, 8, 0, 0);
  if (d10000000 != -1) {
    Icon_Draw(d10000000 + 140, 152, 8, 0, 0);
  }
  if (d1000000 != -1) {
    Icon_Draw(d1000000 + 140, 160, 8, 0, 0);
  }
  if (d100000 != -1) {
    Icon_Draw(d100000 + 140, 168, 8, 0, 0);
  }
  if (d10000 != -1) {
    Icon_Draw(d10000 + 140, 176, 8, 0, 0);
  }
  if (d1000 != -1) {
    Icon_Draw(d1000 + 140, 184, 8, 0, 0);
  }
  if (d100 != -1) {
    Icon_Draw(d100 + 140, 192, 8, 0, 0);
  }
  if (d10 != -1) {
    Icon_Draw(d10 + 140, 200, 8, 0, 0);
  }
  if (rest != -1) {
    Icon_Draw(rest + 140, 208, 8, 0, 0);
  }
  Screen_SetCurrent(saved027c);
  Screen_SetCurrentGfx(saved0280);
}
