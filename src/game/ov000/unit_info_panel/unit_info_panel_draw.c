#include "types.h"
#include "game/Unk_02080440.h"
#include "game/unit_info_panel.h"
#include "game/unit_stats.h"

extern Unk_02080440* data_027c027c;

void ScriptSlot_ClearCallback(int slot);
void UnitInfoPanel_DrawUnit(void);
void UnitInfoPanel_DrawAmmoAndRange(void);

/* Script callback of the panel: draws the unit each frame. With no unit it
 * clears the callback of the running script slot. */
void UnitInfoPanel_Draw(void) {
  MapUnit* unit = data_ov000_0234ec8c->unit;

  if (unit == NULL || unit->type == UNIT_NONE) {
    ScriptSlot_ClearCallback((u8)*data_027c027c->scriptSlotCurrent);
    return;
  }
  UnitInfoPanel_DrawUnit();
  UnitInfoPanel_DrawAmmoAndRange();
}
