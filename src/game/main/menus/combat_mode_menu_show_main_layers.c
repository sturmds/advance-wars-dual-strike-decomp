#include "types.h"
#include "game/menu_button.h"
#include "game/Unk_02080440.h"
#include "game/Unk_02088d04.h"
#include "game/screen_other_map.h"

void Gfx_ClearLayerMap(int layer);
void Gfx_RequestLayerCopy(int layer);
void HelpText_Show(u32 textId, int tile);

/* Clears the buttons and sets the layers shown on the main screen: BG0, BG2,
 * BG3 and OBJ on, BG1 off. Then it clears layer 0 and shows the help text. */
void CombatModeMenu_ShowMainLayers(void) {
  Unk_027c0280* saved0280;
  Unk_02080440* saved027c;

  MenuButtons_RemoveAll();

  saved0280 = data_027c0280;
  Screen_SetCurrentGfx(data_027c0288.unk_00);
  saved027c = data_027c027c;
  Screen_SetCurrent(data_027c0288.unk_00);
  data_027c0280->dispCnt |= 0x100;
  data_027c0280->dispCnt &= ~0x200;
  data_027c0280->dispCnt |= 0x400;
  data_027c0280->dispCnt |= 0x800;
  data_027c0280->dispCnt |= 0x1000;
  Gfx_ClearLayerMap(0);
  Gfx_RequestLayerCopy(0);
  Screen_SetCurrent(saved027c);
  Screen_SetCurrentGfx(saved0280);

  HelpText_Show(0x44000000, 0x3c0);
}
