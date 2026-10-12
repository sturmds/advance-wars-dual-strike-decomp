#include "types.h"
#include "game/Unk_02080440.h"
#include "game/map_unit.h"
#include "game/terrain.h"
#include "game/unit_stats.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;

int GetUnitMovement(int player, int unitType);
void RangeMap_SetBuffer(void* tileMap);
void RangeMap_FillUnitMoves(MapUnit* unit);
BOOL Unit_IsCurrentPlayers(s16 unitSlot);
int UnitType_GetAttackKind(int unitType);

#define STATE (data_027c027c->unk_48[data_027c0284])
#define MAP (STATE->mapTiles)
#define TILE_INDEX(x, y) ((x) + MAP->rowStart[y])

#define NO_COST 0x7fff
#define NO_TILE 9999

/* Picks the shoal or port where the acting sea transport picks up
 * footUnit. Unk_02080440_48.rangeMap holds the transport's moves (filled
 * by the caller); MapTiles.rangeScratch gets footUnit's moves here, and
 * the tile must be in both. Among free tiles it keeps the cheapest for the
 * transport: a port at plain cost when the transport's fuel is more than
 * 20 below full, any other tile at cost plus footUnit's movement. With no
 * free tile, it takes the cheapest one holding a current player's unit
 * that is not a ground unit and not indirect. Returns 1 with the tile in
 * dest[0] (x) and dest[1] (y), or 0 when there is none. */
int AiSeaTransport_PickBoardingTile(MapUnit* footUnit, s16* dest) {
  int occupiedCost;
  int terrain;
  int slot;
  int movement;
  int bestCost;
  s16 occupiedX;
  int x;
  int y;
  s16 occupiedY;

  bestCost = NO_COST;
  occupiedCost = bestCost;
  movement = GetUnitMovement(STATE->currentPlayer, footUnit->type);
  RangeMap_SetBuffer(MAP->rangeScratch);
  RangeMap_FillUnitMoves(footUnit);
  RangeMap_SetBuffer(MAP->rangeMapTiles);
  dest[0] = NO_TILE;
  occupiedX = NO_TILE;
  for (y = 0; y < STATE->unk_1c4->mapHeight; y++) {
    for (x = 0; x < STATE->unk_1c4->mapWidth; x++) {
      if (STATE->rangeMap[y][x] < 0) {
        continue;
      }
      if (MAP->rangeScratch[TILE_INDEX(x, y)] < 0) {
        continue;
      }
      terrain = MAP_TILE_TERRAIN(MAP->tiles[TILE_INDEX(x, y)]);
      if (terrain != TERRAIN_SHOAL && terrain != TERRAIN_PORT) {
        continue;
      }
      slot = MAP->unitAt[TILE_INDEX(x, y)];
      if (slot != 0) {
        if (!Unit_IsCurrentPlayers(slot)) {
          continue;
        }
        if (Ov000_gUnitStats[STATE->units[slot].type].moveGroup &
            (MOVE_GROUP_FOOT | MOVE_GROUP_TIRES | MOVE_GROUP_TREAD)) {
          continue;
        }
        if (UnitType_GetAttackKind(STATE->units[slot].type) ==
            ATTACK_KIND_INDIRECT) {
          continue;
        }
        if (STATE->rangeMap[y][x] > occupiedCost) {
          continue;
        }
        occupiedX = x;
        occupiedCost = STATE->rangeMap[y][x];
        occupiedY = y;
      } else if (terrain == TERRAIN_PORT &&
                 Ov000_gUnitStats[STATE->actingUnit->type].fuel >
                     STATE->actingUnit->fuel + 20) {
        if (STATE->rangeMap[y][x] > bestCost) {
          continue;
        }
        dest[0] = x;
        dest[1] = y;
        bestCost = STATE->rangeMap[y][x];
      } else {
        if (movement + STATE->rangeMap[y][x] > bestCost) {
          continue;
        }
        dest[0] = x;
        dest[1] = y;
        bestCost = movement + STATE->rangeMap[y][x];
      }
    }
  }
  if (dest[0] == NO_TILE) {
    /* The original tests occupiedY, which is never set when no
         * occupied tile was found; only occupiedX starts at NO_TILE. */
    if (occupiedY == NO_TILE) {
      return 0;
    }
    dest[0] = occupiedX;
    dest[1] = occupiedY;
  }
  return 1;
}
