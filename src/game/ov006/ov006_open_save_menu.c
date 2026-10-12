#include "types.h"
#include "game/popup_menu.h"
#include "game/Unk_ov006_0235f690.h"

PopupMenuTask* PopupMenu_Create(const MenuItem* items, int x, int y, int style,
                                int selected, int center);
int DesignMap_CopyName(int id, u8* buf);
void Ov006_DrawMenuLineFixedText(int unused, int row);
void Ov006_DrawMenuLineWithIcon(int unused, int row, const char* text);

extern Unk_ov006_0235f690* data_ov006_0236d640;
extern const MenuItem data_ov006_0236ba60[];

/* Opens the menu of the three design map slots with the cursor on the last
 * chosen slot. Each row shows the slot's map name with an icon, or a fixed
 * text when the slot is empty. Choosing a row runs Ov006_StartIdScript, which
 * also takes an empty slot, so this looks like the save menu (the load menu,
 * Ov006_OpenLoadMenu, only takes a filled slot). */
void Ov006_OpenSaveMenu(void) {
  /* chosenSlot is 0xff (-1 here) when none was chosen. */
  s8 slot = data_ov006_0236d640->chosenSlot;

  PopupMenu_Create(data_ov006_0236ba60, 2, 5, 0, slot < 0 ? 0 : slot, 0);
  if (DesignMap_CopyName(0, &data_ov006_0236d640->unk_eb) != 1) {
    Ov006_DrawMenuLineFixedText(1, 0);
  } else {
    Ov006_DrawMenuLineWithIcon(1, 0, (const char*)&data_ov006_0236d640->unk_eb);
  }
  if (DesignMap_CopyName(1, &data_ov006_0236d640->unk_eb) != 1) {
    Ov006_DrawMenuLineFixedText(1, 1);
  } else {
    Ov006_DrawMenuLineWithIcon(1, 1, (const char*)&data_ov006_0236d640->unk_eb);
  }
  if (DesignMap_CopyName(2, &data_ov006_0236d640->unk_eb) != 1) {
    Ov006_DrawMenuLineFixedText(1, 2);
  } else {
    Ov006_DrawMenuLineWithIcon(1, 2, (const char*)&data_ov006_0236d640->unk_eb);
  }
}
