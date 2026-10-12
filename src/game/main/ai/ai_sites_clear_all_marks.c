#include "types.h"
#include "game/ai_build_state.h"
#include "game/ai_site.h"

/* Sets AiSite.kind back to 127 on every site marked 0xfe (no action
 * found) or 0xff (rejected for a unit type). */
void AiSites_ClearAllMarks(void) {
  int i;
  AiSite* site = data_0229e5bc;

  for (i = 0; i < data_021619c0->recordCount; i++, site++) {
    if (site->kind >= 254) {
      site->kind = 127;
    }
  }
}
