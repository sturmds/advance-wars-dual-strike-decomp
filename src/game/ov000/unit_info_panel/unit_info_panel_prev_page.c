#include "types.h"
#include "game/unit_info_panel.h"

extern s8 data_ov000_022b3850[];

/* Page number, 0 to UNIT_INFO_PANEL_PAGES - 1 */
#define PAGE (data_ov000_0234ec8c->page)

void Sound_PlaySe(u32 soundId);

/* Like the next-page step, going backwards (wrapping from 0 to 15). */
void UnitInfoPanel_PrevPage(void) {
  Sound_PlaySe(0x10002);
  PAGE--;
  if (PAGE < 0) {
    PAGE = UNIT_INFO_PANEL_PAGES - 1;
  }
  while (
      data_ov000_0234fa28[data_ov000_022b3850[data_ov000_0234ec8c->unit->type] -
                          1][PAGE] == 0) {
    PAGE--;
    if (PAGE < 0) {
      PAGE = UNIT_INFO_PANEL_PAGES - 1;
    }
  }
}
