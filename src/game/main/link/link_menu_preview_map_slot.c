#include "types.h"
#include "game/menu_button.h"
#include "game/menu_state.h"
#include "game/screen_other_map.h"

extern MenuState gMenuState;

void Sound_PlaySe(u32 soundId);
void MapPreview_DrawRecord(u8* dst, DesignMapRecord* record);

/* Draws the map of a stage 2 button (ids 2-4 are map slots 0-2) into the preview area of the
 * screen data_027c0288.unk_04. */
void LinkMenu_PreviewMapSlot(MenuButton* button) {
  Unk_027c0280* saved0280;
  Unk_02080440* saved027c;

  Sound_PlaySe(0x10002);
  saved0280 = data_027c0280;
  Screen_SetCurrentGfx(data_027c0288.unk_04);
  saved027c = data_027c027c;
  Screen_SetCurrent(data_027c0288.unk_04);
  MapPreview_DrawRecord(data_027c0280->objVram + 0x8000,
                        &LINK_MAPS[button->id - 2]);
  Screen_SetCurrent(saved027c);
  Screen_SetCurrentGfx(saved0280);
}
