#include "types.h"
#include "game/unit_info_panel.h"

extern const u8* data_ov000_022f5d1c[];

void UnitInfoPanel_DrawIconPair(int slot, int code, int arg, int link);

/* Walks the unit type's byte list in data_ov000_022f5d1c and calls
 * UnitInfoPanel_DrawIconPair (which draws icons) once per pair of bytes (code,
 * argument). 6 restarts the slot count at 4 and 7 ends the list. link is 13
 * when an even code is followed by the same code with bit 0 set, -1 when an
 * odd code follows the matching even one; otherwise 0. */
void UnitInfoPanel_DrawIconList(void) {
  const u8* list = data_ov000_022f5d1c[data_ov000_0234ec8c->unit->type];
  u8 i = 0;
  u8 slot = 0;
  int link;
  u8 code;
  u8 arg;

  while (list[i] != 7) {
    code = list[i];
    if (code == 6) {
      slot = 4;
      i++;
      continue;
    }
    arg = list[i + 1];
    link = 0;
    if ((code & 1) == 0) {
      if (code == (list[i + 2] & ~1)) {
        link = 13;
      }
    } else if (i >= 2 && list[i - 1] != 6 && (code & ~1) == list[i - 2]) {
      link = -1;
    }
    UnitInfoPanel_DrawIconPair(slot, code, arg, link);
    i += 2;
    slot++;
  }
}
