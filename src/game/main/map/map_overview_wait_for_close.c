#include "types.h"
#include "game/Unk_02080440.h"
#include "game/keypad.h"

extern Unk_02080440* data_027c027c;

void func_020c4024(void);
void func_020c40d0(void);
void MapView_ScrollStep(int directions);
BOOL MapView_IsAtRest(void);
void Sound_PlaySe(u32 soundId);
void ScriptSlot_ClearCallback(int slot);

/* Script callback while the Start-button map overview shows: keeps the
 * cursor and view scrolling. When the view is at rest and A, B or Start was
 * pressed, plays the close sound and ends the callback. */
void MapOverview_WaitForClose(void) {
  func_020c4024();
  func_020c40d0();
  MapView_ScrollStep(8);
  if (!MapView_IsAtRest()) {
    return;
  }
  if ((gKeypad->pressed & (PAD_BUTTON_A | PAD_BUTTON_B | PAD_BUTTON_START)) ==
      0) {
    return;
  }
  Sound_PlaySe(0x10009);
  ScriptSlot_ClearCallback(*data_027c027c->scriptSlotCurrent & 0xff);
}
