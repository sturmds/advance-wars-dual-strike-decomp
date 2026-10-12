#include "types.h"
#include "game/screen_other_map.h"
#include "game/script_slot.h"

void func_01ff9ac4(void);
void func_ov000_022b1a40(void);
void UnitInfoPanel_PutFrameBlock(void);
void UnitInfoPanel_DrawContents(void);

/* Redraws the unit info panel on the other screen. The caller passes a script slot; it is not used. */
void UnitInfoPanel_Redraw(ScriptSlot* slot) {
  Unk_027c0280* saved0280;

  func_ov000_022b1a40();
  saved0280 = data_027c0280;
  Screen_SetCurrentGfx(data_027c0288.unk_04);
  func_01ff9ac4();
  UnitInfoPanel_PutFrameBlock();
  UnitInfoPanel_DrawContents();
  Screen_SetCurrentGfx(saved0280);
}
