#include "types.h"
#include "game/popup_menu.h"
#include "game/Unk_ov006_0235f690.h"

PopupMenuTask* PopupMenu_Create(const MenuItem* items, int x, int y, int style,
                                int selected, int center);
int DesignMap_CopyName(int id, u8* buf);
void Ov006_DrawMenuLineFixedText(int unused, int row);
void Ov006_DrawMenuLineWithIcon(int unused, int row, const char* text);

extern Unk_ov006_0235f690* data_ov006_0236d640;
extern const MenuItem data_ov006_0236b9e0[];

/* Opens the menu of the three design map slots with the cursor on the last
 * chosen slot, drawn like Ov006_OpenSaveMenu. Choosing a row starts a script
 * (func_ov006_02357eac) only when the slot holds a map (flag 0x200 << slot),
 * so this looks like the load menu. */
void Ov006_OpenLoadMenu(void) {
  /* chosenSlot is 0xff (-1 here) when none was chosen. */
  s8 slot = data_ov006_0236d640->chosenSlot;

  PopupMenu_Create(data_ov006_0236b9e0, 2, 5, 0, slot < 0 ? 0 : slot, 0);
  if (DesignMap_CopyName(0, &data_ov006_0236d640->unk_eb) != 1) {
    Ov006_DrawMenuLineFixedText(0, 0);
  } else {
    Ov006_DrawMenuLineWithIcon(0, 0, (const char*)&data_ov006_0236d640->unk_eb);
  }
  if (DesignMap_CopyName(1, &data_ov006_0236d640->unk_eb) != 1) {
    Ov006_DrawMenuLineFixedText(0, 1);
  } else {
    Ov006_DrawMenuLineWithIcon(0, 1, (const char*)&data_ov006_0236d640->unk_eb);
  }
  if (DesignMap_CopyName(2, &data_ov006_0236d640->unk_eb) != 1) {
    Ov006_DrawMenuLineFixedText(0, 2);
  } else {
    Ov006_DrawMenuLineWithIcon(0, 2, (const char*)&data_ov006_0236d640->unk_eb);
  }
}
