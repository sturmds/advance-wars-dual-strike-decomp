#include "types.h"
#include "game/ai_build_state.h"
#include "game/ai_site.h"

int AiSite_NearUnguardedOozium(int x, int y);

/* Picks the site of a given type (SITE_TYPE_ANY: every type) with the lowest
 * unk_09, the range-map value of the nearest Infantry or Mech. Returns the
 * site index, or -1. */
int AiSite_PickNearestFoot(int siteType) {
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
