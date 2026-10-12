#include "types.h"
#include "game/Unk_0229fb30.h"
#include "game/Unk_02160ac0.h"
#include "game/menu_button.h"
#include "game/screen_other_map.h"

extern Unk_02160ac0* data_02160ac0;

void func_02082a8c(int palette);
void func_02088528(int a0);
void HelpText_Show(u32 textId, int tile);

/* Screen setup: runs func_02082a8c(19) on the second screen, moves the cursor rect off screen,
 * selects the button saved in data_02160ac0->unk_7e and shows a help text. */
void func_02087c20(void) {
  Unk_027c0280* saved0280 = data_027c0280;
  Unk_02080440* saved027c;

  Screen_SetCurrentGfx(data_027c0288.unk_04);
  saved027c = data_027c027c;
  Screen_SetCurrent(data_027c0288.unk_04);
  func_02082a8c(19);
  Screen_SetCurrent(saved027c);
  Screen_SetCurrentGfx(saved0280);
  MenuCursor_SetRect(0, -16, 256, -16);
  MenuButton_Select(data_02160ac0->unk_7e);
  if (data_0229fb30.unk_45a != 0) {
    func_02088528(0);
  }
  HelpText_Show(0x44000014, 0x3c0);
}
