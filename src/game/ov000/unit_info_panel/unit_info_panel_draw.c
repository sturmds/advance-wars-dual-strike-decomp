#include "types.h"
#include "game/Unk_02080440.h"
#include "game/unit_info_panel.h"
#include "game/unit_stats.h"

extern Unk_02080440 *data_027c027c;

void func_0201710c(int slot);
void func_ov000_022b2838(void);
void func_ov000_022b2a3c(void);

/* Script callback of the panel: draws the unit each frame. With no unit it
 * clears the callback of the running script slot. */
void UnitInfoPanel_Draw(void)
{
    MapUnit *unit = data_ov000_0234ec8c->unit;

    if (unit == NULL || unit->type == UNIT_NONE) {
        func_0201710c((u8)*data_027c027c->scriptSlotCurrent);
        return;
    }
    func_ov000_022b2838();
    func_ov000_022b2a3c();
}
