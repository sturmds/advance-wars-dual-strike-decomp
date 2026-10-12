#include "types.h"
#include "game/screen_other_map.h"
#include "game/script_slot.h"

s8 ScriptSlot_StopByScript(const void* descriptor);
void UnitInfoPanel_ClearUnit(void);

extern ScriptCommand data_ov000_0234f2dc[];
extern ScriptCommand data_ov000_0234f30c[];

/* Closes the panel: stops its two scripts and drops its unit. */
void UnitInfoPanel_Close(void) {
  Unk_027c0280* saved0280;
  Unk_02080440* saved027c;

  saved0280 = data_027c0280;
  Screen_SetCurrentGfx(data_027c0288.unk_00);
  saved027c = data_027c027c;
  Screen_SetCurrent(data_027c0288.unk_00);
  ScriptSlot_StopByScript(data_ov000_0234f2dc);
  ScriptSlot_StopByScript(data_ov000_0234f30c);
  Screen_SetCurrent(saved027c);
  Screen_SetCurrentGfx(saved0280);
  UnitInfoPanel_ClearUnit();
}
