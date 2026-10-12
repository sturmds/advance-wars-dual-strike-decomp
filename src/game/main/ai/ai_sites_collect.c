#include "types.h"
#include "game/ai_site.h"
#include "game/terrain.h"
#include "game/Unk_02080440.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;

extern u8 gTerrainFactoryKind
    []; /* per terrain id: 1 base, 2 airport, 3 port, else 0 */

BOOL Map_IsFreeDeployCity(int x, int y);

#define STATE (data_027c027c->unk_48[data_027c0284])

#define TILE_TERRAIN(x, y) \
  MAP_TILE_TERRAIN(STATE->mapTiles->tiles[(x) + STATE->mapTiles->rowStart[y]])

/* Writes one AiSite record for each property that Map_IsFreeDeployCity accepts
 * and ends the list with x 0xff. Returns the number of records. A city counts
 * as a base. */
int AiSites_Collect(void) {
  int count = 0;
  Unk_02080440_6088* property;
  AiSite* site = data_0229e5bc;

  for (property = STATE->unk_6088; property->terrain != 0xff; property++) {
    if (Map_IsFreeDeployCity(property->x, property->y) != 1) {
      continue;
    }
    site->x = property->x;
    site->y = property->y;
    site->type = gTerrainFactoryKind[TILE_TERRAIN(property->x, property->y)];
    if (TILE_TERRAIN(property->x, property->y) == TERRAIN_CITY) {
      site->type = SITE_BASE;
    }
    site->kind = 127;
    site->group = -1;
    site->unitBalance = 0;
    site->enemyCount = 0;
    site->enemyDistance = 0xff;
    site->propertyMoves = 0xff;
    site->unk_09 = 0xff;
    site->unk_0a = 0xff;
    site->score = 0;
    site++;
    count++;
  }
  site->x = 0xff;
  return count;
}
