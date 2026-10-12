#include "types.h"
#include "game/ai_build_state.h"
#include "game/ai_site_group.h"

/* Clears AiSiteGroup.unk_01 in every group. Purpose unknown. */
void func_020937dc(void) {
  int i;
  AiSiteGroup* slot = gAiSiteGroups;

  for (i = 0; i < data_021619c0->slotCount; i++, slot++) {
    slot->unk_01 = 0;
  }
}
