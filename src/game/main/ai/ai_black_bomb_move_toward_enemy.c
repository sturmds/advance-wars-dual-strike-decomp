#include "types.h"
#include "game/Unk_02080440.h"
#include "game/map_unit.h"
#include "game/unit_stats.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;

BOOL Player_IsInGame(u8 player);
void RangeMap_FillUnitMoves(MapUnit* unit);
void RangeMap_MarkAdjacentTiles(s8 mark);
int Unit_GetValuePerHp(int playerIndex, int unitType);
int Map_GetDistance(int x1, int y1, int x2, int y2);
void AiUnit_MoveTowardTile(int x, int y);

#define STATE (data_027c027c->unk_48[data_027c0284])

/* Moves the acting Black Bomb toward an enemy unit it cannot reach this
 * turn. Prefers the nearest such unit that is not carried, has over 60 hp
 * and costs at least a quarter of a Black Bomb per hp. Without one, goes
 * for the nearest enemy unit. The marks 110 to 112 probably cover the
 * tiles next to the reachable area, so a negative rangeMap value
 * means out of reach. */
void AiBlackBomb_MoveTowardEnemy(void) {
  int valuableX;
  int valuableY;
  int nearestX;
  int nearestY;
  int i;
  int bestDist;
  int bestValuableDist;
  int player;
  int slot;
  MapUnit* unit;
  int dist;

  valuableX = -1;
  valuableY = valuableX;
  nearestX = valuableX;
  nearestY = valuableX;
  RangeMap_FillUnitMoves(STATE->actingUnit);
  for (i = 0; i < 3; i++) {
    RangeMap_MarkAdjacentTiles(110 + i);
  }
  bestDist = 0x7fff;
  bestValuableDist = 0x7fff;
  for (player = 1; player < 5; player++) {
    if (!Player_IsInGame(player)) {
      continue;
    }
    if (STATE->players[STATE->currentPlayer].team ==
        STATE->players[player].team) {
      continue;
    }
    for (slot = Ov000_gPlayerFirstUnitSlot[player] + 1;
         slot < Ov000_gPlayerFirstUnitSlot[player] + 51; slot++) {
      unit = &STATE->units[slot];
      if (unit->type == UNIT_NONE) {
        continue;
      }
      dist = Map_GetDistance(STATE->actingUnit->x, STATE->actingUnit->y,
                             unit->x, unit->y);
      if (bestDist > dist) {
        bestDist = dist;
        nearestX = unit->x;
        nearestY = unit->y;
      }
      if (STATE->rangeMap[unit->y][unit->x] >= 0) {
        continue;
      }
      if (unit->flags & MAP_UNIT_CARRIED) {
        continue;
      }
      if (unit->hp < 61) {
        continue;
      }
      if (Unit_GetValuePerHp(STATE->currentPlayer, unit->type) <
          Unit_GetValuePerHp(STATE->currentPlayer, UNIT_BLACK_BOMB) >> 2) {
        continue;
      }
      dist = Map_GetDistance(STATE->actingUnit->x, STATE->actingUnit->y,
                             unit->x, unit->y);
      if (bestValuableDist > dist) {
        valuableX = unit->x;
        valuableY = unit->y;
        bestValuableDist = dist;
      }
    }
  }
  if (bestValuableDist == 0x7fff) {
    if (bestDist == 0x7fff) {
      return;
    }
    AiUnit_MoveTowardTile(nearestX, nearestY);
  } else {
    AiUnit_MoveTowardTile(valuableX, valuableY);
  }
}
