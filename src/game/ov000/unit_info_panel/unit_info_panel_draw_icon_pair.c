#include "types.h"
#include "game/icon.h"
#include "game/unit_info_panel.h"

extern u16 data_ov000_0234f004[];

/* Draws two icons at the panel position that data_ov000_0234f004 (x, y pairs)
 * gives for slot. Called with the entry of a byte list and the entry after it;
 * the fourth argument is not used. The name is a guess. */
void UnitInfoPanel_DrawIconPair(int slot, int entry, int next, int unused) {
  int base = (entry & ~1) + 90;

  Icon_Draw(next + base,
            data_ov000_0234ec8c->originX + data_ov000_0234f004[slot * 2] + 1,
            data_ov000_0234f004[slot * 2 + 1], 0, 0);
  Icon_Draw(entry + 96,
            data_ov000_0234ec8c->originX + data_ov000_0234f004[slot * 2] + 1,
            data_ov000_0234f004[slot * 2 + 1], 0, 0);
}
