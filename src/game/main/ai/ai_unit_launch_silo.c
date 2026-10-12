#include "types.h"
#include "game/ai_unit_action.h"
#include "game/Unk_02080440.h"
#include "game/strike_target.h"
#include "game/terrain.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;

void RangeMap_FillUnitMoves(MapUnit* unit);
void AiUnit_PerformAction(s16 x, s16 y, int action, u8 arg3, u8 arg4);

#define STATE (data_027c027c->unk_48[data_027c0284])
#define TILE(x, y) ((x) + STATE->mapTiles->rowStart[y])

/* Picks the free silo the acting unit reaches with the highest rangeMap
 * value, and launches it at the site Strike_FindBestSite finds. Does
 * nothing when no silo is in reach or no site passes the funds check. */
void AiUnit_LaunchSilo(void) {
  int siloX;
  int siloY;
  int bestReach;
  s16 site[2];
  int x;
  int y;
  int reach;

  siloY = -1;
  siloX = siloY;
  bestReach = siloY;
  RangeMap_FillUnitMoves(STATE->actingUnit);
  for (y = 0; y < STATE->unk_1c4->mapHeight; y++) {
    for (x = 0; x < STATE->unk_1c4->mapWidth; x++) {
      if (STATE->rangeMap[y][x] < 0) {
        continue;
      }
      if (MAP_TILE_TERRAIN(STATE->mapTiles->tiles[TILE(x, y)]) !=
          TERRAIN_SILO) {
        continue;
      }
      if (STATE->mapTiles->unitAt[TILE(x, y)] != 0) {
        continue;
      }
      reach = STATE->rangeMap[y][x];
      if (reach > bestReach) {
        siloX = x;
        siloY = y;
        bestReach = reach;
      }
    }
  }
  if (bestReach == -1) {
    return;
  }
  site[0] = siloX;
  site[1] = siloY;
  if (Strike_FindBestSite(site, STATE->currentPlayer, 1,
                          STRIKE_MODE_AI_FUNDS) != 1) {
    return;
  }
  AiUnit_PerformAction(siloX, siloY, AI_ACTION_LAUNCH_SILO, site[0], site[1]);
}
