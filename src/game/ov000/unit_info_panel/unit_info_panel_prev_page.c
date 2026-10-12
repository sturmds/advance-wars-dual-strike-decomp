#include "types.h"
#include "game/unit_info_panel.h"

extern s8 data_ov000_022b3850[];
extern const void* data_ov000_0234fa28[][16];

/* Page number 0-15 */
#define PAGE (data_ov000_0234ec8c->page)

void Sound_PlaySe(u32 soundId);

/* Like the next-page step, going backwards (wrapping from 0 to 15). */
void UnitInfoPanel_PrevPage(void) {
  Sound_PlaySe(0x10002);
  PAGE--;
  if (PAGE < 0) {
    PAGE = 15;
  }
  while (
      data_ov000_0234fa28[data_ov000_022b3850[data_ov000_0234ec8c->unit->type] -
                          1][PAGE] == NULL) {
    PAGE--;
    if (PAGE < 0) {
      PAGE = 15;
    }
  }
}
