#include "types.h"
#include "game/Unk_02080440.h"
#include "game/ai_unit_action.h"
#include "game/map_unit.h"
#include "game/terrain.h"
#include "game/unit_stats.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;

int GetUnitMovement(int player, int unitType);
void RangeMap_FillMoves(int x, int y, int unitType, int movement, int arg4);
void RangeMap_SetBuffer(void* tileMap);
BOOL Ai_HasOwnBaseWithin(int x, int y, int radius);
void AiUnit_MoveTowardTile(int x, int y);

#define STATE (data_027c027c->unk_48[data_027c0284])
#define MAP (STATE->mapTiles)
#define TILE_INDEX(x, y) ((x) + MAP->rowStart[y])

#define UNLIMITED_MOVE 110
#define NO_TILE 0x7fff

/* Run last by AiSeaTransport_GoToFootUnit for an empty sea transport with
 * no foot unit to fetch. It picks the current player's port it can reach
 * for the lowest move cost; a port that fails Ai_HasOwnBaseWithin (radius 5)
 * costs 256 more. Around that port (MapTiles.rangeScratch: a Tank's moves
 * from it) it then picks a free tile it can reach: a port when its
 * fuel is more than 20 below full, or a shoal, which costs its own
 * movement more. It heads there, or to the port when no tile was found
 * and the port is out of reach this turn (gAiTurnState->unk_05). */
void AiSeaTransport_WaitNearPort(void) {
  int bestX;
  int bestCost;
  int bestY;
  int movement;
  int x;
  int y;
  int cost;
  int portX;
  int portY;

  bestX = -1;
  bestY = -1;
  bestCost = NO_TILE;
  movement = GetUnitMovement(STATE->currentPlayer, STATE->actingUnit->type);
  RangeMap_FillMoves(STATE->actingUnit->x, STATE->actingUnit->y,
                     STATE->actingUnit->type, UNLIMITED_MOVE, 0);
  for (portY = 0; portY < STATE->unk_1c4->mapHeight; portY++) {
    for (portX = 0; portX < STATE->unk_1c4->mapWidth; portX++) {
      cost = STATE->rangeMap[portY][portX];
      if (cost < 0) {
        continue;
      }
      if (MAP_TILE_TERRAIN(MAP->tiles[TILE_INDEX(portX, portY)]) !=
          TERRAIN_PORT) {
        continue;
      }
      if (STATE->unk_04 != (MAP->tiles[TILE_INDEX(portX, portY)] & 0xe0)) {
        continue;
      }
      if (Ai_HasOwnBaseWithin(portX, portY, 5) == 0) {
        cost += 256;
      }
      if (bestCost >= cost) {
        bestX = portX;
        bestY = portY;
        bestCost = cost;
      }
    }
  }
  if (bestX == -1) {
    return;
  }
  RangeMap_SetBuffer(MAP->rangeScratch);
  RangeMap_FillMoves(bestX, bestY, UNIT_TANK,
                     GetUnitMovement(STATE->currentPlayer, UNIT_TANK), 0);
  RangeMap_SetBuffer(MAP->rangeMapTiles);
  bestCost = NO_TILE;
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
      if (MAP_TILE_TERRAIN(MAP->tiles[TILE_INDEX(x, y)]) == TERRAIN_PORT &&
          Ov000_gUnitStats[STATE->actingUnit->type].fuel >
              STATE->actingUnit->fuel + 20) {
        if (bestCost >= STATE->rangeMap[y][x]) {
          bestX = x;
          bestY = y;
          bestCost = STATE->rangeMap[y][x];
        }
      } else if (MAP_TILE_TERRAIN(MAP->tiles[TILE_INDEX(x, y)]) ==
                 TERRAIN_SHOAL) {
        if (bestCost >= movement + STATE->rangeMap[y][x]) {
          bestX = x;
          bestY = y;
          bestCost = movement + STATE->rangeMap[y][x];
        }
      }
    }
  }
  if (bestCost == NO_TILE &&
      STATE->rangeMap[bestY][bestX] <= gAiTurnState->unk_05) {
    return;
  }
  AiUnit_MoveTowardTile(bestX, bestY);
}
