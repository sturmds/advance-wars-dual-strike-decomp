#include "types.h"
#include "game/Unk_02080440.h"
#include "game/map_unit.h"
#include "game/terrain.h"
#include "game/unit_stats.h"
#include "game/range_map.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;

void RangeMap_FillMoves(int x, int y, int unitType, int movement, int arg4);
BOOL Ai_CanEndMoveAt(s16 x, s16 y);
int Ai_IsTileNeededByAlly(s16 x, s16 y);
void AiUnit_MoveTowardTile(int x, int y);

#define STATE (data_027c027c->unk_48[data_027c0284])
#define MAP (STATE->mapTiles)
#define TILE_INDEX(x, y) ((x) + MAP->rowStart[y])

#define UNLIMITED_MOVE 110
#define NO_TILE 0x7fff

/* Run by AiEmptyTransport_Act after AiEmptyTransport_GoToFootUnit, for an
 * empty transport: moves it towards the closest tile (by its own move cost)
 * within 2 Infantry moves of a base of the current player, so it is near
 * when new foot units are built. A base with a unit that already waited is
 * skipped. MapTiles.rangeScratch holds the Infantry distance to such a base,
 * -1 elsewhere. */
void AiEmptyTransport_WaitNearBase(void) {
  int bestMoves = NO_TILE;
  int bestX = -1;
  int bestY = 0;
  int baseX;
  int baseY;
  int nearX;
  int nearY;
  int x;
  int y;

  if (STATE->actingUnit->cargo[0] != 0) {
    return;
  }
  RangeMap_SetBuffer(MAP->rangeScratch);
  RangeMap_Fill(-1);
  RangeMap_SetBuffer(MAP->rangeMapTiles);
  for (baseY = 0; baseY < STATE->unk_1c4->mapHeight; baseY++) {
    for (baseX = 0; baseX < STATE->unk_1c4->mapWidth; baseX++) {
      if (MAP_TILE_TERRAIN(MAP->tiles[TILE_INDEX(baseX, baseY)]) !=
          TERRAIN_BASE) {
        continue;
      }
      if (STATE->unk_04 != (MAP->tiles[TILE_INDEX(baseX, baseY)] & 0xe0)) {
        continue;
      }
      if (MAP->unitAt[TILE_INDEX(baseX, baseY)] != 0 &&
          (STATE->units[MAP->unitAt[TILE_INDEX(baseX, baseY)]].flags &
           MAP_UNIT_WAITED)) {
        continue;
      }
      RangeMap_FillMoves(baseX, baseY, UNIT_INFANTRY, 2, 1);
      for (nearY = 0; nearY < STATE->unk_1c4->mapHeight; nearY++) {
        for (nearX = 0; nearX < STATE->unk_1c4->mapWidth; nearX++) {
          if (STATE->rangeMap[nearY][nearX] >= 0) {
            MAP->rangeScratch[TILE_INDEX(nearX, nearY)] =
                STATE->rangeMap[nearY][nearX];
          }
        }
      }
    }
  }
  RangeMap_FillMoves(STATE->actingUnit->x, STATE->actingUnit->y,
                     STATE->actingUnit->type, UNLIMITED_MOVE, 1);
  for (y = 0; y < STATE->unk_1c4->mapHeight; y++) {
    for (x = 0; x < STATE->unk_1c4->mapWidth; x++) {
      if (STATE->rangeMap[y][x] < 0) {
        continue;
      }
      if (MAP->rangeScratch[TILE_INDEX(x, y)] < 0) {
        continue;
      }
      if (MAP->unitAt[TILE_INDEX(x, y)] != 0) {
        continue;
      }
      if (!Ai_CanEndMoveAt(x, y)) {
        continue;
      }
      if (Ai_IsTileNeededByAlly(x, y) == 1) {
        continue;
      }
      if (STATE->rangeMap[y][x] <= bestMoves) {
        bestX = x;
        bestY = y;
        bestMoves = STATE->rangeMap[y][x];
      }
    }
  }
  if (bestX == -1) {
    return;
  }
  AiUnit_MoveTowardTile(bestX, bestY);
}
