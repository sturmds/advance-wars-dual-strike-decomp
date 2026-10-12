#include "types.h"
#include "game/ai_build_state.h"
#include "game/ai_site.h"

/* Sets AiSite.kind back to 127 on every site that AiTurn_PickUnitToBuild
 * marked 0xff while trying out a unit type. */
void AiSites_ClearRejectedMarks(void) {
  int i;
  AiSite* site;

  site = data_0229e5bc;
  for (i = 0; i < data_021619c0->recordCount; i++, site++) {
    if (site->kind == 0xff) {
      site->kind = 127;
    }
  }
}
