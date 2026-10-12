#include "types.h"
#include "game/ai_build_state.h"
#include "game/ai_site.h"
#include "game/ai_site_group.h"

int AiSite_NearUnguardedOozium(int x, int y);

#define GROUP_PENALTY 255

/* Like AiSite_PickNearestEnemy without the group test, but a base site whose
 * group has unk_03_2 set counts GROUP_PENALTY further away. */
int AiSite_PickNearestEnemyPenalized(int siteType) {
  AiSite* site;
  int i;
  int bestDistance;
  int best;
  int distance;

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
    distance = site->enemyDistance;
    if (site->type == SITE_BASE && gAiSiteGroups[site->group].unk_03_2) {
      distance += GROUP_PENALTY;
    }
    if (bestDistance > distance) {
      bestDistance = distance;
      best = i;
    }
  }
  return best;
}
