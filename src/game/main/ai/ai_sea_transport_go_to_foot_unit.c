#include "types.h"
#include "game/Unk_02080440.h"
#include "game/ai_unit_action.h"
#include "game/map_unit.h"
#include "game/unit_stats.h"
#include "game/range_map.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;
extern u8 gAiTransportsSentToUnit[];

void RangeMap_FillMoves(int x, int y, int unitType, int movement, int arg4);
void RangeMap_MarkAdjacentTiles(int value);
s32 HwDiv32(s32 numerator, s32 denominator);
BOOL Unit_IsCurrentPlayers(int unitSlot);
void AiUnit_PerformAction(s16 x, s16 y, int action, u8 arg3, u8 arg4);
int AiSeaTransport_FindPickupTile(int x, int y, s16* dest);
int AiSeaTransport_PickBoardingTile(MapUnit* unit, s16* dest);
void AiUnit_MoveTowardTile(int x, int y);
void AiBlackBoat_GoToUnitInNeed(void);
void AiBlackBoat_RepairDamagedUnit(void);
void AiSeaTransport_WaitNearPort(void);

#define STATE (data_027c027c->unk_48[data_027c0284])
#define MAP (STATE->mapTiles)
#define TILE_INDEX(x, y) ((x) + MAP->rowStart[y])
#define FIRST_UNIT (Ov000_gPlayerFirstUnitSlot[STATE->currentPlayer] + 1)

#define UNLIMITED_MOVE 110
#define NO_TILE 0x7fff

/* Run by AiSeaTransport_Move for an empty sea transport (the acting unit).
 * Scores each of the current player's units that wait for a transport
 * (transportWanted 2, only Infantry and Mechs for a Black Boat) and that
 * the transport can reach: a lower score is better. The score rises with the
 * transport's move cost to the pick-up tile (AiSeaTransport_FindPickupTile) and falls by one
 * for each other waiting unit within 4 tiles. The best unit gets the
 * transport, which moves to the pick-up tile this turn if it can and
 * AiUnit_MoveTowardTile heads for it otherwise. With no unit to fetch, the routines
 * for the idle case run. */
void AiSeaTransport_GoToFootUnit(void) {
  int maxY;
  int maxX;
  int dist;
  int minX;
  int y;
  MapUnit* neighbour;
  int score;
  MapUnit* unit;
  int bestX;
  int minY;
  s16 dest[2];
  int bestY;
  int bestScore;
  int i;
  int x;

  bestX = -1;
  bestY = -1;
  bestScore = NO_TILE;
  RangeMap_FillMoves(STATE->actingUnit->x, STATE->actingUnit->y,
                     STATE->actingUnit->type, UNLIMITED_MOVE, 1);
  RangeMap_MarkAdjacentTiles(111);
  for (i = FIRST_UNIT; i < FIRST_UNIT + 50; i++) {
    unit = &STATE->units[i];
    if (unit->type == UNIT_NONE || (unit->flags & MAP_UNIT_CARRIED)) {
      continue;
    }
    if (STATE->rangeMap[unit->y][unit->x] < 0) {
      continue;
    }
    if (unit->transportWanted != 2) {
      continue;
    }
    if (gAiTransportsSentToUnit[i & 63] != 0) {
      continue;
    }
    if (STATE->actingUnit->type == UNIT_BLACK_BOAT && unit->type > UNIT_MECH) {
      continue;
    }
    if (AiSeaTransport_FindPickupTile(unit->x, unit->y, dest) == 0) {
      continue;
    }
    score =
        (HwDiv32(STATE->rangeMap[dest[1]][dest[0]], gAiTurnState->unk_05) + 1)
        << 7;
    minX = unit->x - 4;
    if (minX < 0) {
      minX = 0;
    }
    minY = unit->y - 4;
    if (minY < 0) {
      minY = 0;
    }
    maxX = STATE->unk_1c4->mapWidth - 1;
    if (unit->x + 4 < maxX) {
      maxX = unit->x + 4;
    }
    maxY = STATE->unk_1c4->mapHeight - 1;
    if (unit->y + 4 < maxY) {
      maxY = unit->y + 4;
    }
    for (y = minY; y <= maxY; y++) {
      for (x = minX; x <= maxX; x++) {
        dist = 0;
        if (unit->x - x > 0) {
          dist += unit->x - x;
        } else {
          dist += x - unit->x;
        }
        if (unit->y - y > 0) {
          dist += unit->y - y;
        } else {
          dist += y - unit->y;
        }
        if (dist < 0 || dist > 4) {
          continue;
        }
        neighbour = &STATE->units[MAP->unitAt[TILE_INDEX(x, y)]];
        if (neighbour->type == UNIT_NONE) {
          continue;
        }
        if (!Unit_IsCurrentPlayers(MAP->unk_508[TILE_INDEX(x, y)])) {
          continue;
        }
        if (neighbour->transportWanted == 2) {
          score--;
        }
      }
    }
    if (bestScore >= score) {
      bestScore = score;
      bestX = unit->x;
      bestY = unit->y;
    }
  }
  if (bestX != -1 &&
      AiSeaTransport_PickBoardingTile(
          &STATE->units[MAP->unitAt[TILE_INDEX(bestX, bestY)]], dest) == 1) {
    gAiTransportsSentToUnit[MAP->unitAt[TILE_INDEX(bestX, bestY)] & 63]++;
    if (MAP->unitAt[TILE_INDEX(dest[0], dest[1])] == 0 &&
        STATE->rangeMap[dest[1]][dest[0]] <= gAiTurnState->unk_05) {
      AiUnit_PerformAction(dest[0], dest[1], AI_ACTION_WAIT, 0, 0);
    } else {
      AiUnit_MoveTowardTile(dest[0], dest[1]);
    }
    return;
  }
  if (STATE->actingUnit->type == UNIT_BLACK_BOAT) {
    AiBlackBoat_RepairDamagedUnit();
    AiBlackBoat_GoToUnitInNeed();
  }
  AiSeaTransport_WaitNearPort();
}
