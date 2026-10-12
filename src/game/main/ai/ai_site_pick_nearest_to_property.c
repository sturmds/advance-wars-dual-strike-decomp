#include "types.h"
#include "game/ai_build_state.h"
#include "game/ai_site.h"

int AiSite_NearUnguardedOozium(int x, int y);

/* Among the sites that are not skipped and not rejected by AiSite_NearUnguardedOozium,
 * with the given site type (SITE_TYPE_ANY: every type) and group, picks the
 * one with the lowest propertyMoves. Returns the site index, or -1. */
int AiSite_PickNearestToProperty(int siteType, int group) {
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
    if (site->group != group) {
      continue;
    }
    if (bestMoves > site->propertyMoves) {
      bestMoves = site->propertyMoves;
      best = i;
    }
  }
  return best;
}
