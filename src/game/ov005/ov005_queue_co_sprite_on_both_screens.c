#include "types.h"
#include "game/screen_other_map.h"
#include "game/Unk_ov005_0236bba8.h"

void func_0200270c(int x, int y, int attr, int list, int hFlip);
void func_02002694(int x, int y, int attr, int list, int hFlip);

/* Queues the same sprite (attributes 0xca00) at x, y on one screen and 64
 * pixels higher on the other. `work` is not read. The second block uses its
 * own saved-state locals to match. */
void Ov005_QueueCoSpriteOnBothScreens(Unk_ov005_0236bba8* work, int x, int y) {
  Unk_027c0280* saved0280 = data_027c0280;
  Unk_02080440* saved027c;
  Unk_027c0280* savedSub0280;
  Unk_02080440* savedSub027c;

  Screen_SetCurrentGfx(data_027c0288.unk_04);
  saved027c = data_027c027c;
  Screen_SetCurrent(data_027c0288.unk_04);
  x &= 0x1ff;
  func_0200270c(x, y & 0xff, 0xca00, 0, 0);
  Screen_SetCurrent(saved027c);
  Screen_SetCurrentGfx(saved0280);

  savedSub0280 = data_027c0280;
  Screen_SetCurrentGfx(data_027c0288.unk_00);
  savedSub027c = data_027c027c;
  Screen_SetCurrent(data_027c0288.unk_00);
  func_02002694(x, (y - 64) & 0xff, 0xca00, 0, 0);
  Screen_SetCurrent(savedSub027c);
  Screen_SetCurrentGfx(savedSub0280);
}
