#include "types.h"
#include "game/ai_build_state.h"
#include "game/ai_site.h"

int AiSite_NearUnguardedOozium(int x, int y);

/* Like AiSite_PickNearestToProperty, but picks the site with the most
 * enemy units near it (enemyCount). Returns the site index, or -1. */
int AiSite_PickMostEnemies(int siteType, int group) {
  AiSite* site;
  int i;
  int bestCount;
  int best;

  bestCount = 0;
  i = 0;
  site = data_0229e5bc;
  best = -1;
  for (; i < data_021619c0->recordCount; i++, site++) {
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
    if (bestCount < site->enemyCount) {
      bestCount = site->enemyCount;
      best = i;
    }
  }
  return best;
}
