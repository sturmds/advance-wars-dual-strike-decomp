#include "types.h"
#include "game/Unk_0229fb30.h"
#include "game/menu_button.h"
#include "game/menu_state.h"
#include "game/screen_other_map.h"

extern MenuState gMenuState;
extern const MenuButtonTemplate data_02160150;

int func_0207a260(void);
int LinkMenu_IsMapSlotUsed(int player, int index);
void MapPreview_DrawRecord(u8* dst, DesignMapRecord* record);

/* Adds the stage 2 buttons (ids 2-4, map slots 0-2, in this link player's
 * column) the way LinkMenu_RestoreStage2Buttons does, selects the first one
 * added and draws its map into the preview area. */
void LinkMenu_AddStage2Buttons(void) {
  int first = 255;
  int i;
  Unk_027c0280* saved0280;
  Unk_02080440* saved027c;

  for (i = 0; i < LINK_DESIGN_MAPS_PER_PLAYER; i++) {
    if (data_0229fb30.unk_631 != func_0207a260() ||
        LinkMenu_IsMapSlotUsed(func_0207a260(), i) != 0) {
      MenuButton_Add(func_0207a260() * 96 + 40, i * 16 + 120, &data_02160150,
                     i + 2);
      if (first > i + 2) {
        first = i + 2;
      }
    }
  }
  MenuButton_Select(first);
  saved0280 = data_027c0280;
  Screen_SetCurrentGfx(data_027c0288.unk_04);
  saved027c = data_027c027c;
  Screen_SetCurrent(data_027c0288.unk_04);
  MapPreview_DrawRecord(data_027c0280->objVram + 0x8000, &LINK_MAPS[first - 2]);
  Screen_SetCurrent(saved027c);
  Screen_SetCurrentGfx(saved0280);
}
