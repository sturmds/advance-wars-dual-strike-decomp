#include "types.h"
#include "game/ai_build_state.h"
#include "game/ai_site.h"

int AiSite_NearUnguardedOozium(int x, int y);

/* AiSite_PickNearestToProperty without the group test. */
int AiSite_PickNearestToPropertyAnyGroup(int siteType) {
  AiSite* site;
  int i;
  int bestMoves;
  int best;

  site = data_0229e5bc;
  bestMoves = 0x7fff;
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
    if (bestMoves > site->propertyMoves) {
      bestMoves = site->propertyMoves;
      best = i;
    }
  }
  return best;
}
