#include "types.h"
#include "game/menu_button.h"
#include "game/ov005_task.h"
#include "game/screen_other_map.h"
#include "game/Unk_ov005_0236bba8.h"

int Co_GetRank(int coId);

extern const MenuButtonTemplate data_ov005_0236abd4[];

/* Adds the nine palette buttons; the last (variant 8) only when Co_GetRank is
 * 10 or more. Then selects the button of the CO's current palette variant. */
void Ov005_AddCoPaletteButtons(Ov005Task* task) {
  Unk_ov005_0236bba8* work = task->work;
  int i;
  Unk_027c0280* saved0280;
  Unk_02080440* saved027c;
  CoPaletteButton* extra = work->paletteButtons;

  for (i = 0; i < CO_PALETTE_BUTTONS; i++, extra++) {
    if (Co_GetRank(work->coId) >= 10 || i != 8) {
      saved0280 = data_027c0280;
      Screen_SetCurrentGfx(data_027c0288.unk_00);
      saved027c = data_027c027c;
      Screen_SetCurrent(data_027c0288.unk_00);
      MenuButton_Add(extra->x, extra->y, data_ov005_0236abd4, extra->buttonId);
      Screen_SetCurrent(saved027c);
      Screen_SetCurrentGfx(saved0280);
    }
  }
  MenuButton_Select(work->paletteButtons[work->variant].buttonId);
}
