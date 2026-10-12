#include "types.h"
#include "game/ai_build_state.h"
#include "game/ai_site.h"

int AiSite_NearUnguardedOozium(int x, int y);

/* Like AiSite_PickNearestToProperty, but picks the site whose nearest
 * enemy unit is closest (enemyDistance). Returns the site index, or -1. */
int AiSite_PickNearestEnemy(int siteType, int group) {
  AiSite* site;
  int i;
  int bestDistance;
  int best;

  site = data_0229e5bc;
  bestDistance = 0x7fff;
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
    if (bestDistance > site->enemyDistance) {
      bestDistance = site->enemyDistance;
      best = i;
    }
  }
  return best;
}
