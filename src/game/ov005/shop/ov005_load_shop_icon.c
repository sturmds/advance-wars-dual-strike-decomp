#include "types.h"
#include "game/icon.h"
#include "game/screen_other_map.h"
#include "game/Unk_ov005_0236bbb0.h"

/* Loads an icon once: at most 50 are kept, and an id already in the work
 * block's list is skipped. */
void Ov005_LoadShopIcon(Unk_ov005_0236bbb0* work, int iconId) {
  u32 i;
  Unk_027c0280* saved0280;
  Unk_02080440* saved027c;

  if (work->iconCount >= 50) {
    return;
  }
  for (i = 0; i < work->iconCount; i++) {
    if (iconId == work->icons[i]) {
      return;
    }
  }
  saved0280 = data_027c0280;
  Screen_SetCurrentGfx(data_027c0288.unk_00);
  saved027c = data_027c027c;
  Screen_SetCurrent(data_027c0288.unk_00);
  Icon_Load(iconId);
  Screen_SetCurrent(saved027c);
  Screen_SetCurrentGfx(saved0280);
  work->icons[i] = iconId;
  work->iconCount++;
}
