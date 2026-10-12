#include "types.h"
#include "game/Unk_02080440.h"
#include "game/Unk_02088d04.h"

void Screen_SetCurrent(void* state);
void Screen_SetCurrentGfx(void* state);
void func_0200270c(int x, int y, int attr, int list, int hFlip);

extern Unk_02080440* data_027c027c;
extern Unk_027c0280* data_027c0280;
extern Unk_027c0288 data_027c0288;

/* Queues one sprite (attribute 0xa580, list 5) at x, y on the second screen.
 * `state` is not used. */
void Ov005SoundRoom_QueueSprite(void* state, int x, int y) {
  Unk_027c0280* saved0280 = data_027c0280;
  Unk_02080440* saved027c;

  Screen_SetCurrentGfx(data_027c0288.unk_04);
  saved027c = data_027c027c;
  Screen_SetCurrent(data_027c0288.unk_04);
  func_0200270c(x & 0x1ff, y & 0xff, 0xa580, 5, 0);
  Screen_SetCurrent(saved027c);
  Screen_SetCurrentGfx(saved0280);
}
