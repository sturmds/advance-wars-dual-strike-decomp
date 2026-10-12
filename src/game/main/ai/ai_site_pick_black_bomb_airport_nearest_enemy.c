#include "types.h"
#include "game/Unk_02080440.h"
#include "game/ai_build_state.h"
#include "game/ai_site.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;

#define STATE (data_027c027c->unk_48[data_027c0284])
#define TILE(x, y) ((x) + STATE->mapTiles->rowStart[y])

#define THREAT_LIMIT 50

/* Among the airport sites whose tile has enemy threatMax plus threatSum of
 * at most THREAT_LIMIT, picks the one with the smallest enemyDistance.
 * Returns the site index, or -1. */
int AiSite_PickBlackBombAirportNearestEnemy(void) {
  AiSite* site;
  int i;
  int bestDistance;
  int best;
  int tile;

  site = data_0229e5bc;
  bestDistance = 0x7fff;
  best = -1;
  for (i = 0; i < data_021619c0->recordCount; i++, site++) {
    if (site->kind >= SITE_KIND_SKIPPED) {
      continue;
    }
    tile = TILE(site->x, site->y);
    if (STATE->mapTiles->threatMax[tile] + STATE->mapTiles->threatSum[tile] >
        THREAT_LIMIT) {
      continue;
    }
    if (site->type != SITE_AIRPORT) {
      continue;
    }
    if (bestDistance > site->enemyDistance) {
      bestDistance = site->enemyDistance;
      best = i;
    }
  }
  return best;
}
