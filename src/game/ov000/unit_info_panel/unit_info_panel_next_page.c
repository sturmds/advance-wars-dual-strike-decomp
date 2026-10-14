#include "types.h"
#include "game/unit_info_panel.h"

const s8 data_ov000_022b3850[] = {
#include "data/ov000/data_ov000_022b3850.inc.c"
};

/* Page number, 0 to UNIT_INFO_PANEL_PAGES - 1 */
#define PAGE (data_ov000_0234ec8c->page)

void Sound_PlaySe(u32 soundId);

/* Plays the page sound and steps the panel to the next page (wrapping from 15 to 0)
 * that has an entry in data_ov000_0234fa28 for the unit's type row. The row is
 * data_ov000_022b3850[type] - 1. */
void UnitInfoPanel_NextPage(void) {
  Sound_PlaySe(0x10002);
  PAGE++;
  if (PAGE >= UNIT_INFO_PANEL_PAGES) {
    PAGE = 0;
  }
  while (
      data_ov000_0234fa28[data_ov000_022b3850[data_ov000_0234ec8c->unit->type] -
                          1][PAGE] == 0) {
    PAGE++;
    if (PAGE >= UNIT_INFO_PANEL_PAGES) {
      PAGE = 0;
    }
  }
}
