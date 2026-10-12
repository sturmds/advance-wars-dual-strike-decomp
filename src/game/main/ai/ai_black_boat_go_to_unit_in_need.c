#include "types.h"
#include "game/Unk_02080440.h"
#include "game/map_unit.h"
#include "game/unit_stats.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;

void RangeMap_FillMoves(int x, int y, int unitType, int movement, int arg4);
void RangeMap_MarkAdjacentTiles(int value);
BOOL Unit_IsCurrentPlayers(int unitSlot);
void AiUnit_MoveTowardTile(int x, int y);

#define STATE (data_027c027c->unk_48[data_027c0284])
#define MAP (STATE->mapTiles)
#define TILE_INDEX(x, y) ((x) + MAP->rowStart[y])

#define UNLIMITED_MOVE 110
#define NO_TILE 9999

/* Run by AiSeaTransport_GoToFootUnit for a Black Boat with no foot unit to
 * fetch. Among the current player's units on tiles the boat can reach, it
 * picks three: the damaged ship (hp 90 or less) closest by move cost, the
 * ship other than a Lander or Black Boat with the lowest fuel that is short
 * of full and not on a tile whose owner bits equal unk_04, and the closest
 * damaged unit that is not a ship. AiUnit_MoveTowardTile runs for each one
 * found, in that order. */
void AiBlackBoat_GoToUnitInNeed(void) {
  UnitStats* stats;
  MapUnit* unit;
  s16 groundX;
  int lowestFuel;
  s16 fuelX;
  int closestGround;
  int closestShip;
  int fuel;
  int x;
  s16 shipX;
  s16 shipY;
  s16 fuelY;
  int y;
  s16 groundY;
  int tile;

  RangeMap_FillMoves(STATE->actingUnit->x, STATE->actingUnit->y,
                     STATE->actingUnit->type, UNLIMITED_MOVE, 1);
  RangeMap_MarkAdjacentTiles(111);
  groundX = NO_TILE;
  fuelX = NO_TILE;
  shipX = NO_TILE;
  closestGround = 255;
  closestShip = closestGround;
  lowestFuel = 100;
  for (y = 0; y < STATE->unk_1c4->mapHeight; y++) {
    for (x = 0; x < STATE->unk_1c4->mapWidth; x++) {
      if (STATE->rangeMap[y][x] < 0) {
        continue;
      }
      tile = TILE_INDEX(x, y);
      unit = &STATE->units[MAP->unitAt[tile]];
      if (unit->type == UNIT_NONE || unit->type == UNIT_OOZIUM) {
        continue;
      }
      if (!Unit_IsCurrentPlayers(MAP->unk_508[tile])) {
        continue;
      }
      stats = &Ov000_gUnitStats[unit->type];
      if (stats->moveGroup & MOVE_GROUP_SEA) {
        if (unit->hp <= 90 && STATE->rangeMap[y][x] < closestShip) {
          closestShip = STATE->rangeMap[y][x];
          shipX = x;
          shipY = y;
        }
        if (unit->type == UNIT_LANDER || unit->type == UNIT_BLACK_BOAT) {
          continue;
        }
        if (STATE->unk_04 == (MAP->tiles[TILE_INDEX(x, y)] & 0xe0)) {
          continue;
        }
        fuel = unit->fuel;
        if (fuel == stats->fuel) {
          continue;
        }
        if (fuel < lowestFuel) {
          lowestFuel = fuel;
          fuelX = x;
          fuelY = y;
        }
      } else {
        if (unit->hp <= 90 && STATE->rangeMap[y][x] < closestGround) {
          closestGround = STATE->rangeMap[y][x];
          groundX = x;
          groundY = y;
        }
      }
    }
  }
  if (shipX != NO_TILE) {
    AiUnit_MoveTowardTile(shipX, shipY);
  }
  if (fuelX != NO_TILE) {
    AiUnit_MoveTowardTile(fuelX, fuelY);
  }
  if (groundX != NO_TILE) {
    AiUnit_MoveTowardTile(groundX, groundY);
  }
}
