#include "types.h"
#include "game/Unk_0229fb30.h"
#include "game/menu_button.h"
#include "game/screen_other_map.h"

extern const MenuButtonTemplate data_02160128;

int func_0207a260(void);
int LinkMenu_PlayerHasMap(int index);
void Mem_Zero32(u32 value, void* dest, u32 size);

/* Only on the side where func_0207a260() is 0: adds the stage 1 buttons (ids 0 and 1) that
 * LinkMenu_PlayerHasMap allows, selects the earlier pick, and clears the map preview area (0x4000 bytes at
 * objVram + 0x8000) of the screen data_027c0288.unk_04. */
void LinkMenu_AddStage1Buttons(void) {
  int i;
  Unk_027c0280* saved0280;
  Unk_02080440* saved027c;

  if (func_0207a260() != 0) {
    return;
  }
  for (i = 0; i < 2; i++) {
    if (LinkMenu_PlayerHasMap(i) != 0) {
      MenuButton_Add(40, i * 16 + 56, &data_02160128, i);
    }
  }
  MenuButton_Select(data_0229fb30.unk_631);
  saved0280 = data_027c0280;
  Screen_SetCurrentGfx(data_027c0288.unk_04);
  saved027c = data_027c027c;
  Screen_SetCurrent(data_027c0288.unk_04);
  Mem_Zero32(0, data_027c0280->objVram + 0x8000, 0x4000);
  Screen_SetCurrent(saved027c);
  Screen_SetCurrentGfx(saved0280);
}
