#include "types.h"
#include "game/Unk_02080440.h"
#include "game/ai_build_state.h"
#include "game/ai_site.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;

int AiSite_NearUnguardedOozium(int x, int y);

#define STATE (data_027c027c->unk_48[data_027c0284])
#define TILE(x, y) ((x) + STATE->mapTiles->rowStart[y])

#define SAFE_THREAT_LIMIT 40

/* Picks the airport site to build a Fighter at: not skipped, not one where
 * AiSite_NearUnguardedOozium returns 1, and with both enemy threat values of its tile below
 * SAFE_THREAT_LIMIT. Prefers the most enemies near the site, then the nearest enemy.
 * Returns the site index, or -1. */
int AiSite_PickAirport(void) {
  /* needed to match: declaration and initialisation order */
  AiSite* site;
  int i;
  int bestEnemyCount;
  int bestEnemyDistance;
  int best;
  int tile;

  bestEnemyCount = 0;
  i = 0;
  site = data_0229e5bc;
  bestEnemyDistance = 0x7fff;
  best = -1;
  for (; i < data_021619c0->recordCount; i++, site++) {
    if (site->kind >= SITE_KIND_SKIPPED) {
      continue;
    }
    if (AiSite_NearUnguardedOozium(site->x, site->y) == 1) {
      continue;
    }
    if (site->type != SITE_AIRPORT) {
      continue;
    }
    tile = TILE(site->x, site->y);
    if (STATE->mapTiles->threatMax[tile] >= SAFE_THREAT_LIMIT ||
        STATE->mapTiles->threatSum[tile] >= SAFE_THREAT_LIMIT) {
      continue;
    }
    if (bestEnemyCount > site->enemyCount) {
      continue;
    }
    if (bestEnemyCount == site->enemyCount &&
        bestEnemyDistance < site->enemyDistance) {
      continue;
    }
    bestEnemyDistance = site->enemyDistance;
    bestEnemyCount = site->enemyCount;
    best = i;
  }
  return best;
}
