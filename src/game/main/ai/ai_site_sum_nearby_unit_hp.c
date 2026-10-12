#include "types.h"
#include "game/Unk_02080440.h"
#include "game/ai_profile.h"
#include "game/ai_site.h"
#include "game/unit_stats.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;
extern u16
    gAiEnemyHpByType[UNIT_TYPE_COUNT]; /* per unit type: hp of enemy units */
extern u16 gAiOwnHpByType
    [UNIT_TYPE_COUNT]; /* per unit type: hp of the current player's team */

/* The y byte of each AiSite record (data_0229e5bc + 1); the original
 * addresses it through its own symbol. */
extern u8 data_0229e5bd[][sizeof(AiSite)];

void RangeMap_FillMoves(int x, int y, int unitType, int movement, int arg4);
void AiBuild_WeighUnitTypes(void);

#define STATE (data_027c027c->unk_48[data_027c0284])
#define TILE(x, y) ((x) + STATE->mapTiles->rowStart[y])
#define SITE_RADIUS (gAiProfile->unk_08[8]) /* AiProfile byte 0x10 */

/* Like AiBuild_SumUnitHp, but only counts units within SITE_RADIUS tiles of
 * a site (Manhattan distance). Ground units count only when a Tank could
 * reach their tile from the site within SITE_RADIUS move points. Then calls
 * AiBuild_WeighUnitTypes. */
void AiSite_SumNearbyUnitHp(int siteIndex) {
  /* needed to match: declaration order, the u8 types and the
     * radius-first sums below */
  int maxX;
  int minY;
  MapUnit* unit;
  int y;
  int dist;
  int maxY;
  int x;
  int type;
  int minX;
  u8 slot;
  int siteX;
  u8 siteY;

  for (type = 0; type < UNIT_TYPE_COUNT; type++) {
    gAiOwnHpByType[type] = gAiEnemyHpByType[type] = 0;
  }
  RangeMap_FillMoves(data_0229e5bc[siteIndex].x, data_0229e5bd[siteIndex][0],
                     UNIT_TANK, SITE_RADIUS, 0);

  siteX = data_0229e5bc[siteIndex].x;
  minX = siteX - SITE_RADIUS;
  if (minX < 0) {
    minX = 0;
  }
  siteY = data_0229e5bd[siteIndex][0];
  minY = siteY - SITE_RADIUS;
  if (minY < 0) {
    minY = 0;
  }
  maxX = STATE->unk_1c4->mapWidth - 1;
  if (SITE_RADIUS + siteX < maxX) {
    maxX = SITE_RADIUS + siteX;
  }
  maxY = STATE->unk_1c4->mapHeight - 1;
  if (SITE_RADIUS + siteY < maxY) {
    maxY = SITE_RADIUS + siteY;
  }
  for (y = minY; y <= maxY; y++) {
    for (x = minX; x <= maxX; x++) {
      dist = 0;
      if (data_0229e5bc[siteIndex].x - x > 0) {
        dist += data_0229e5bc[siteIndex].x - x;
      } else {
        dist += x - data_0229e5bc[siteIndex].x;
      }
      if (data_0229e5bc[siteIndex].y - y > 0) {
        dist += data_0229e5bc[siteIndex].y - y;
      } else {
        dist += y - data_0229e5bc[siteIndex].y;
      }
      if (dist < 0 || dist > SITE_RADIUS) {
        continue;
      }
      slot = STATE->mapTiles->unitAt[TILE(x, y)];
      if (slot == 0) {
        continue;
      }
      unit = &STATE->units[slot];
      type = unit->type;
      if ((Ov000_gUnitStats[type].moveGroup &
           (MOVE_GROUP_FOOT | MOVE_GROUP_TIRES | MOVE_GROUP_TREAD)) &&
          STATE->rangeMap[y][x] < 0) {
        continue;
      }
      if (STATE->players[STATE->currentPlayer].team ==
          STATE->players[(slot >> 6) + 1].team) {
        gAiOwnHpByType[type] += unit->hp;
      } else {
        gAiEnemyHpByType[type] += unit->hp;
      }
    }
  }
  AiBuild_WeighUnitTypes();
}
