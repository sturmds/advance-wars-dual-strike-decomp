#include "types.h"
#include "game/ai_build_state.h"
#include "game/ai_site.h"
#include "game/ai_site_group.h"

int AiSite_NearUnguardedOozium(int x, int y);

/* AiSite_PickNearestFoot, limited to sites whose group has a property in
 * reach that the current team does not own (AiSiteGroup.propertyCount). */
int AiSite_PickNearestFootWithProperty(int siteType) {
  AiSite* site;
  int i;
  int bestValue;
  int best;

  site = data_0229e5bc;
  bestValue = 0x7fff;
  best = -1;
  for (i = 0; i < data_021619c0->recordCount; i++, site++) {
    if (site->kind >= SITE_KIND_SKIPPED) {
      continue;
    }
    if (gAiSiteGroups[site->group].propertyCount == 0) {
      continue;
    }
    if (AiSite_NearUnguardedOozium(site->x, site->y) == 1) {
      continue;
    }
    if (siteType != SITE_TYPE_ANY && site->type != siteType) {
      continue;
    }
    if (bestValue > site->unk_09) {
      bestValue = site->unk_09;
      best = i;
    }
  }
  return best;
}
