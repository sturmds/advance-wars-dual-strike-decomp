#include "types.h"
#include "game/Unk_02080440.h"
#include "game/map_unit.h"
#include "game/unit_stats.h"
#include "game/ai/ai_unit_move.h"
#include "game/ai_unit_action.h"
#include "game/ai/ai_capture_site.h"
#include "game/ai/ai_tile_search.h"
#include "game/terrain.h"
#include "game/range_map.h"
#include "game/ai/ai_enemy_within.h"
#include "game/math.h"
#include "game/ai_site.h"
#include "game/ai_site_group.h"
#include "game/sound_arc.h"
#include "game/unit_action.h"
#include "sdk/wl.h"
#include "game/ai_build_state.h"
#include "game/ai_profile.h"
#include "game/ai/ai_sea_units.h"
#include "game/ai/ai_unload.h"
#include "game/ai/ai_perform_action.h"
#include "game/ai.h"

/* The functions are in reverse address order: mwccarm emits a file's
 * functions last to first. */

#define UNLIMITED_MOVE 110
#define NO_TILE_POS 9999     /* an x or y no tile has: none found */
#define FULL_SHOWN_HP_MIN 91 /* hp 91-100 shows as 10 */
#define NO_TILE 0x7fff
#define FIRST_UNIT (Ov000_gPlayerFirstUnitSlot[MAP_STATE->currentPlayer] + 1)
#define NO_COST 0x7fff
#define CARGO_UNIT(slot) (&MAP_STATE->units[MAP_STATE->actingUnit->cargo[slot]])

void (*gAiQueueRoutine)(void);
/* run on each entry */
int (*data_0229e174)(int a, int b, int c);
u8* gAiQueueCursor;
/* write position in gAiQueue */
BOOL (*gAiUnloadTest)(void);
int (*gAiUnloadScore)(void);
AiTurnState data_0229e178;
AiBuildState data_0229e184;
UnitAction gUnitAction;
u8 gAiTransportsSentToUnit[MAP_UNITS_PER_PLAYER];
/* sort key of each gAiQueue entry, same index; size: up to the next symbol */
u8 gAiQueueSortKeys[AI_QUEUE_SIZE];
u8 gAiQueue[AI_QUEUE_SIZE];
/* per unit type: build score; 0: not a candidate; size: up to the next symbol */
u32 gAiBuildScores[0x1b];
AiProfile data_0229e2e4;
AiSiteGroup gAiSiteGroups[0x5c];
/* size: up to the next symbol */
AiSite data_0229e5bc[0x5c];

void RangeMap_FillMoves(int x, int y, int unitType, int movement, int arg4);
void RangeMap_MarkAdjacentTiles(int value);
BOOL Unit_IsCurrentPlayers(s16 unitSlot);
void RangeMap_FillUnitMoves(MapUnit* unit);
int Unit_GetValuePerHp(int playerIndex, int unitType);
int UnitType_GetAttackKind(u8 unitType);
int GetUnitMovement(int player, int unitType);
BOOL func_0209dd7c(void);
BOOL Player_HasHq(int player);
void func_020b5604(void);
BOOL func_020b4da8(int x, int y);
/* size: up to the next symbol */
int func_020b4d04(void);

void func_020b4ca4(void) {
  gAiUnloadTest = AiSeaTransport_CargoReachesTarget;
  gAiUnloadScore = func_020b4d04;
  AiSeaTransport_ScoreUnloadTiles();
  if (gAiTurnState->stepFlags & AI_STEP_UNLOAD_SCORED) {
    func_020b5bf4();
  }
  func_020b5464();
}

/* Tile test for AiTransport_SecondCargoHasTaskAt's order 8-10 routine: fills
 * the range map from (x, y) for the unit in the acting transport's second
 * cargo slot. TRUE when that unit has a target and the map reaches it. */
BOOL AiTransport_SecondCargoReachesTargetAt(int x, int y) {
  MapUnit* units = MAP_STATE->units;
  int slot = MAP_STATE->actingUnit->cargo[1];
  MapUnit* carried = &units[slot];

  RangeMap_FillMoves(x, y, carried->type, AiUnit_GetUnloadMovement(carried), 0);
  if (carried->aiTargetX == MAP_UNIT_NO_TARGET) {
    return FALSE;
  }
  return MAP_STATE->rangeMap[carried->aiTargetY][carried->aiTargetX] > 0;
}

/* Called by AiTransport_UnloadAt when the acting transport stops at (x, y) to
 * unload: TRUE when its second cargo unit should get off there too. The
 * unit's own order picks the check, like AiSeaTransport_MoveLoaded picks
 * a routine (no check exists for order 7: the original calls NULL). If
 * that check fails and the order has no fixed target (not 8, 9 or 10),
 * the checks for orders 0, 3, 4 and 2 are tried in turn; the first that
 * passes becomes the unit's order and clears its target. */
BOOL AiTransport_SecondCargoHasTaskAt(int x, int y) {
  BOOL (*check)(int, int);
  MapUnit* unit = &MAP_STATE->units[MAP_STATE->actingUnit->cargo[1]];

  if (unit->type <= UNIT_MECH || unit->type == UNIT_APC) {
    check = AiSeaTransport_FewFootUnitsForProperties;
  } else {
    switch (unit->aiOrder) {
      case 0:
      case 1:
        check = AiSeaTransport_FewArmedUnitsInRange;
        break;
      case 2:
        check = func_020b51ec;
        break;
      case 3:
      case 5:
        check = func_020b50bc;
        break;
      case 4:
        check = AiSeaTransport_FootUnitsOutnumberArmed;
        break;
      case 6:
        check = func_020b4da8;
        break;
      case 8:
      case 9:
      case 10:
        check = AiTransport_SecondCargoReachesTargetAt;
        break;
      default:
        check = NULL;
        break;
    }
  }
  if (check(x, y) == TRUE) {
    return TRUE;
  }
  switch (unit->aiOrder) {
    case 8:
    case 9:
    case 10:
      return FALSE;
  }
  if (AiSeaTransport_FewArmedUnitsInRange(x, y) == TRUE) {
    unit->aiOrder = 0;
    unit->aiTargetX = MAP_UNIT_NO_TARGET;
    unit->aiTargetY = MAP_UNIT_NO_TARGET;
    return TRUE;
  }
  if (func_020b50bc(x, y) == TRUE) {
    unit->aiOrder = 3;
    unit->aiTargetX = MAP_UNIT_NO_TARGET;
    unit->aiTargetY = MAP_UNIT_NO_TARGET;
    return TRUE;
  }
  if (AiSeaTransport_FootUnitsOutnumberArmed(x, y) == TRUE) {
    unit->aiOrder = 4;
    unit->aiTargetX = MAP_UNIT_NO_TARGET;
    unit->aiTargetY = MAP_UNIT_NO_TARGET;
    return TRUE;
  }
  if (func_020b51ec(x, y) != TRUE) {
    return FALSE;
  }
  unit->aiOrder = 2;
  unit->aiTargetX = MAP_UNIT_NO_TARGET;
  unit->aiTargetY = MAP_UNIT_NO_TARGET;
  return TRUE;
}

/* Moves the acting sea transport when it carries a unit. The cargo slots
 * are first ordered, with AI_STEP_SWAP_CARGO set to undo it afterwards:
 * a foot unit in slot 1 goes before a vehicle in slot 0, and an APC in
 * slot 0 goes behind whatever is in slot 1. Then the unit in slot 0
 * picks the routine: foot units and APCs use func_020b5604, the others
 * go by their AI order, like AiUnit_ActByOrder. */
void AiSeaTransport_MoveLoaded(void) {
  u8 swap;
  u8 first;
  MapUnit* unit;

  if (MAP_STATE->actingUnit->cargo[1] != 0 &&
      CARGO_UNIT(1)->type <= UNIT_MECH && CARGO_UNIT(0)->type > UNIT_MECH) {
    swap = MAP_STATE->actingUnit->cargo[0];
    MAP_STATE->actingUnit->cargo[0] = MAP_STATE->actingUnit->cargo[1];
    MAP_STATE->actingUnit->cargo[1] = swap;
    gAiTurnState->stepFlags |= AI_STEP_SWAP_CARGO;
  }
  first = MAP_STATE->actingUnit->cargo[0];
  unit = &MAP_STATE->units[first];
  if (unit->type == UNIT_APC && MAP_STATE->actingUnit->cargo[1] != 0) {
    MAP_STATE->actingUnit->cargo[0] = MAP_STATE->actingUnit->cargo[1];
    MAP_STATE->actingUnit->cargo[1] = first;
    /* needed to match: unit is read before the flag store, so the store
         * makes case 6 below read the state again */
    unit = CARGO_UNIT(0);
    gAiTurnState->stepFlags |= AI_STEP_SWAP_CARGO;
  }
  if (unit->type <= UNIT_MECH || unit->type == UNIT_APC) {
    func_020b5604();
  } else {
    switch (unit->aiOrder) {
      case 0:
      case 1:
        func_020b5464();
        break;
      case 2:
        if (func_0209dd7c() == TRUE) {
          func_020b5268();
        } else {
          func_020b5464();
        }
        break;
      case 3:
      case 5:
        func_020b5138();
        break;
      case 4:
        func_020b501c();
        break;
      case 6:
        if (Player_HasHq(MAP_STATE->currentPlayer) == TRUE) {
          func_020b4e24();
        } else {
          func_020b5138();
        }
        break;
      case 8:
      case 9:
      case 10:
        func_020b4ca4();
        break;
    }
  }
  if (gAiTurnState->stepFlags & AI_STEP_SWAP_CARGO) {
    swap = MAP_STATE->actingUnit->cargo[0];
    MAP_STATE->actingUnit->cargo[0] = MAP_STATE->actingUnit->cargo[1];
    MAP_STATE->actingUnit->cargo[1] = swap;
    gAiTurnState->stepFlags &= ~AI_STEP_SWAP_CARGO;
  }
}

/* Picks the tile where the acting sea transport can take aboard the unit
 * at (x, y): one of the four next to it, a shoal or port, with a
 * rangeMap value from 0 to UNLIMITED_MOVE (it takes the lowest, the later tile
 * on ties). A tile with a unit on it counts only when the unit is the
 * current player's, does not move on the ground and is not indirect.
 * Writes the tile to dest[0], dest[1]; FALSE when there is none. */
BOOL AiSeaTransport_FindPickupTile(int x, int y, s16* dest) {
  /* declaration order needed to match (register allocation) */
  int maxY;
  int maxX;
  int ty;
  int minX;
  int dist;
  int slot;
  int tx;
  u8 type;
  MapTiles* map;
  int minY;

  dest[0] = NO_TILE_POS;
  minX = x - 1;
  if (minX < 0) {
    minX = 0;
  }
  minY = y - 1;
  if (minY < 0) {
    minY = 0;
  }
  maxX = MAP_STATE->unk_1c4->mapWidth - 1;
  if (x + 1 < maxX) {
    maxX = x + 1;
  }
  maxY = MAP_STATE->unk_1c4->mapHeight - 1;
  if (y + 1 < maxY) {
    maxY = y + 1;
  }
  for (ty = minY; ty <= maxY; ty++) {
    for (tx = minX; tx <= maxX; tx++) {
      dist = 0;
      if (x - tx > 0) {
        dist += x - tx;
      } else {
        dist += tx - x;
      }
      if (y - ty > 0) {
        dist += y - ty;
      } else {
        dist += ty - y;
      }
      if (dist < 1 || dist > 1) {
        continue;
      }
      if (MAP_STATE->rangeMap[ty][tx] < 0 ||
          MAP_STATE->rangeMap[ty][tx] > UNLIMITED_MOVE) {
        continue;
      }
      map = MAP_STATE->mapTiles;
      if (MAP_TILE_TERRAIN(map->tiles[MAP_TILE_INDEX(tx, ty)]) !=
              TERRAIN_SHOAL &&
          MAP_TILE_TERRAIN(map->tiles[MAP_TILE_INDEX(tx, ty)]) !=
              TERRAIN_PORT) {
        continue;
      }
      slot = map->unitAt[MAP_TILE_INDEX(tx, ty)];
      if (slot != 0) {
        if (!Unit_IsCurrentPlayers(slot)) {
          continue;
        }
        type = MAP_STATE->units[slot].type;
        if (Ov000_gUnitStats[type].moveGroup &
            (MOVE_GROUP_FOOT | MOVE_GROUP_TIRES | MOVE_GROUP_TREAD)) {
          continue;
        }
        if (UnitType_GetAttackKind(type) == ATTACK_KIND_INDIRECT) {
          continue;
        }
      }
      if (dest[0] == NO_TILE_POS || MAP_STATE->rangeMap[ty][tx] <=
                                        MAP_STATE->rangeMap[dest[1]][dest[0]]) {
        dest[0] = tx;
        dest[1] = ty;
      }
    }
  }
  return dest[0] != NO_TILE_POS;
}

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
  movement = GetUnitMovement(MAP_STATE->currentPlayer, footUnit->type);
  RangeMap_SetBuffer(MAP_TILES->rangeScratch);
  RangeMap_FillUnitMoves(footUnit);
  RangeMap_SetBuffer(MAP_TILES->rangeMapTiles);
  dest[0] = NO_TILE_POS;
  occupiedX = NO_TILE_POS;
  for (y = 0; y < MAP_STATE->unk_1c4->mapHeight; y++) {
    for (x = 0; x < MAP_STATE->unk_1c4->mapWidth; x++) {
      if (MAP_STATE->rangeMap[y][x] < 0) {
        continue;
      }
      if (MAP_TILES->rangeScratch[MAP_TILE_INDEX(x, y)] < 0) {
        continue;
      }
      terrain = MAP_TILE_TERRAIN(MAP_TILES->tiles[MAP_TILE_INDEX(x, y)]);
      if (terrain != TERRAIN_SHOAL && terrain != TERRAIN_PORT) {
        continue;
      }
      slot = MAP_TILES->unitAt[MAP_TILE_INDEX(x, y)];
      if (slot != 0) {
        if (!Unit_IsCurrentPlayers(slot)) {
          continue;
        }
        if (Ov000_gUnitStats[MAP_STATE->units[slot].type].moveGroup &
            (MOVE_GROUP_FOOT | MOVE_GROUP_TIRES | MOVE_GROUP_TREAD)) {
          continue;
        }
        if (UnitType_GetAttackKind(MAP_STATE->units[slot].type) ==
            ATTACK_KIND_INDIRECT) {
          continue;
        }
        if (MAP_STATE->rangeMap[y][x] > occupiedCost) {
          continue;
        }
        occupiedX = x;
        occupiedCost = MAP_STATE->rangeMap[y][x];
        occupiedY = y;
      } else if (terrain == TERRAIN_PORT &&
                 Ov000_gUnitStats[MAP_STATE->actingUnit->type].fuel >
                     MAP_STATE->actingUnit->fuel + 20) {
        if (MAP_STATE->rangeMap[y][x] > bestCost) {
          continue;
        }
        dest[0] = x;
        dest[1] = y;
        bestCost = MAP_STATE->rangeMap[y][x];
      } else {
        if (movement + MAP_STATE->rangeMap[y][x] > bestCost) {
          continue;
        }
        dest[0] = x;
        dest[1] = y;
        bestCost = movement + MAP_STATE->rangeMap[y][x];
      }
    }
  }
  if (dest[0] == NO_TILE_POS) {
    /* The original tests occupiedY, which is never set when no
         * occupied tile was found; only occupiedX starts at NO_TILE_POS. */
    if (occupiedY == NO_TILE_POS) {
      return 0;
    }
    dest[0] = occupiedX;
    dest[1] = occupiedY;
  }
  return 1;
}

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
  RangeMap_FillMoves(MAP_STATE->actingUnit->x, MAP_STATE->actingUnit->y,
                     MAP_STATE->actingUnit->type, UNLIMITED_MOVE, 1);
  RangeMap_MarkAdjacentTiles(111);
  for (i = FIRST_UNIT; i < FIRST_UNIT + 50; i++) {
    unit = &MAP_STATE->units[i];
    if (unit->type == UNIT_NONE || (unit->flags & MAP_UNIT_CARRIED)) {
      continue;
    }
    if (MAP_STATE->rangeMap[unit->y][unit->x] < 0) {
      continue;
    }
    if (unit->transportWanted != 2) {
      continue;
    }
    if (gAiTransportsSentToUnit[i & 63] != 0) {
      continue;
    }
    if (MAP_STATE->actingUnit->type == UNIT_BLACK_BOAT &&
        unit->type > UNIT_MECH) {
      continue;
    }
    if (AiSeaTransport_FindPickupTile(unit->x, unit->y, dest) == 0) {
      continue;
    }
    score =
        (HwDiv32(MAP_STATE->rangeMap[dest[1]][dest[0]], gAiTurnState->unk_05) +
         1)
        << 7;
    minX = unit->x - 4;
    if (minX < 0) {
      minX = 0;
    }
    minY = unit->y - 4;
    if (minY < 0) {
      minY = 0;
    }
    maxX = MAP_STATE->unk_1c4->mapWidth - 1;
    if (unit->x + 4 < maxX) {
      maxX = unit->x + 4;
    }
    maxY = MAP_STATE->unk_1c4->mapHeight - 1;
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
        neighbour = &MAP_STATE->units[MAP_TILES->unitAt[MAP_TILE_INDEX(x, y)]];
        if (neighbour->type == UNIT_NONE) {
          continue;
        }
        if (!Unit_IsCurrentPlayers(MAP_TILES->unk_508[MAP_TILE_INDEX(x, y)])) {
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
          &MAP_STATE->units[MAP_TILES->unitAt[MAP_TILE_INDEX(bestX, bestY)]],
          dest) == 1) {
    gAiTransportsSentToUnit[MAP_TILES->unitAt[MAP_TILE_INDEX(bestX, bestY)] &
                            63]++;
    if (MAP_TILES->unitAt[MAP_TILE_INDEX(dest[0], dest[1])] == 0 &&
        MAP_STATE->rangeMap[dest[1]][dest[0]] <= gAiTurnState->unk_05) {
      AiUnit_PerformAction(dest[0], dest[1], AI_ACTION_WAIT, 0, 0);
    } else {
      AiUnit_MoveTowardTile(dest[0], dest[1]);
    }
    return;
  }
  if (MAP_STATE->actingUnit->type == UNIT_BLACK_BOAT) {
    AiBlackBoat_RepairDamagedUnit();
    AiBlackBoat_GoToUnitInNeed();
  }
  AiSeaTransport_WaitNearPort();
}

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
  movement =
      GetUnitMovement(MAP_STATE->currentPlayer, MAP_STATE->actingUnit->type);
  RangeMap_FillMoves(MAP_STATE->actingUnit->x, MAP_STATE->actingUnit->y,
                     MAP_STATE->actingUnit->type, UNLIMITED_MOVE, 0);
  for (portY = 0; portY < MAP_STATE->unk_1c4->mapHeight; portY++) {
    for (portX = 0; portX < MAP_STATE->unk_1c4->mapWidth; portX++) {
      cost = MAP_STATE->rangeMap[portY][portX];
      if (cost < 0) {
        continue;
      }
      if (MAP_TILE_TERRAIN(MAP_TILES->tiles[MAP_TILE_INDEX(portX, portY)]) !=
          TERRAIN_PORT) {
        continue;
      }
      if (MAP_STATE->unk_04 !=
          (MAP_TILES->tiles[MAP_TILE_INDEX(portX, portY)] & 0xe0)) {
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
  RangeMap_SetBuffer(MAP_TILES->rangeScratch);
  RangeMap_FillMoves(bestX, bestY, UNIT_TANK,
                     GetUnitMovement(MAP_STATE->currentPlayer, UNIT_TANK), 0);
  RangeMap_SetBuffer(MAP_TILES->rangeMapTiles);
  bestCost = NO_TILE;
  for (y = 0; y < MAP_STATE->unk_1c4->mapHeight; y++) {
    for (x = 0; x < MAP_STATE->unk_1c4->mapWidth; x++) {
      if (MAP_STATE->rangeMap[y][x] < 0) {
        continue;
      }
      if (MAP_TILES->rangeScratch[MAP_TILE_INDEX(x, y)] < 0) {
        continue;
      }
      if (MAP_TILES->unitAt[MAP_TILE_INDEX(x, y)] != 0) {
        continue;
      }
      if (MAP_TILE_TERRAIN(MAP_TILES->tiles[MAP_TILE_INDEX(x, y)]) ==
              TERRAIN_PORT &&
          Ov000_gUnitStats[MAP_STATE->actingUnit->type].fuel >
              MAP_STATE->actingUnit->fuel + 20) {
        if (bestCost >= MAP_STATE->rangeMap[y][x]) {
          bestX = x;
          bestY = y;
          bestCost = MAP_STATE->rangeMap[y][x];
        }
      } else if (MAP_TILE_TERRAIN(MAP_TILES->tiles[MAP_TILE_INDEX(x, y)]) ==
                 TERRAIN_SHOAL) {
        if (bestCost >= movement + MAP_STATE->rangeMap[y][x]) {
          bestX = x;
          bestY = y;
          bestCost = movement + MAP_STATE->rangeMap[y][x];
        }
      }
    }
  }
  if (bestCost == NO_TILE &&
      MAP_STATE->rangeMap[bestY][bestX] <= gAiTurnState->unk_05) {
    return;
  }
  AiUnit_MoveTowardTile(bestX, bestY);
}

/* Black Boat repair priority of a unit: 0 for a unit with no weapon or a
 * Black Bomb, or at 91 HP or more; otherwise by HP: 71-90 gives 4, 51-70
 * gives 5, 31-50 gives 3, 11-30 gives 2 and 1-10 gives 1. */
int AiBlackBoat_GetRepairPriority(MapUnit* unit) {
  if (UnitType_GetAttackKind(unit->type) == ATTACK_KIND_NONE) {
    return 0;
  }
  if (UnitType_GetAttackKind(unit->type) == ATTACK_KIND_BLACK_BOMB) {
    return 0;
  }
  if (unit->hp >= 91) {
    return 0;
  }
  if (unit->hp >= 71) {
    return 4;
  }
  if (unit->hp >= 51) {
    return 5;
  }
  if (unit->hp >= 31) {
    return 3;
  }
  if (unit->hp >= 11) {
    return 2;
  }
  return 1;
}

/* TRUE when unit is a better Black Boat repair target than best (NULL: no
 * best yet). It must have a higher repair priority (AiBlackBoat_GetRepairPriority); on a
 * tie, a higher repair cost per HP. */
BOOL AiBlackBoat_IsBetterRepairTarget(MapUnit* best, MapUnit* unit) {
  int bestPriority;
  int priority;

  if (best == NULL) {
    return TRUE;
  }
  bestPriority = AiBlackBoat_GetRepairPriority(best);
  priority = AiBlackBoat_GetRepairPriority(unit);
  if (bestPriority < priority) {
    return TRUE;
  }
  if (bestPriority == priority) {
    if (Unit_GetValuePerHp(MAP_STATE->currentPlayer, best->type) <
        Unit_GetValuePerHp(MAP_STATE->currentPlayer, unit->type)) {
      return TRUE;
    }
  }
  return FALSE;
}

/* Run by AiSeaTransport_GoToFootUnit for an empty Black Boat. Among the
 * current player's units it can reach next to (not Oozium, not itself)
 * with less than 10 shown HP, and whose one-HP repair cost
 * (Unit_GetValuePerHp) the player can pay, it keeps the one
 * AiBlackBoat_IsBetterRepairTarget prefers and repairs it from an adjacent tile. */
void AiBlackBoat_RepairDamagedUnit(void) {
  /* declaration order needed to match (register allocation) */
  int tile;
  MapUnit* unit;
  s16 bestX;
  MapUnit* bestUnit;
  s16 pos[2];
  int x;
  int y;
  s16 bestY;

  bestUnit = NULL;
  RangeMap_FillUnitMoves(MAP_STATE->actingUnit);
  RangeMap_MarkAdjacentTiles(111);
  bestX = NO_TILE_POS;
  for (y = 0; y < MAP_STATE->unk_1c4->mapHeight; y++) {
    for (x = 0; x < MAP_STATE->unk_1c4->mapWidth; x++) {
      if (MAP_STATE->rangeMap[y][x] < 0) {
        continue;
      }
      tile = MAP_TILE_INDEX(x, y);
      unit = &MAP_STATE->units[MAP_STATE->mapTiles->unitAt[tile]];
      if (unit->type == UNIT_NONE || unit->type == UNIT_OOZIUM) {
        continue;
      }
      if (!Unit_IsCurrentPlayers(MAP_STATE->mapTiles->unk_508[tile])) {
        continue;
      }
      if (unit->hp >= FULL_SHOWN_HP_MIN) {
        continue;
      }
      if (unit == MAP_STATE->actingUnit) {
        continue;
      }
      if ((u32)MAP_STATE->players[MAP_STATE->currentPlayer].funds <
          (u32)Unit_GetValuePerHp(MAP_STATE->currentPlayer, unit->type)) {
        continue;
      }
      if (AiBlackBoat_IsBetterRepairTarget(bestUnit, unit) != TRUE) {
        continue;
      }
      if (FindAttackTile(x, y, pos)) {
        bestX = pos[0];
        bestY = pos[1];
        bestUnit = unit;
      }
    }
  }
  if (bestX == NO_TILE_POS) {
    return;
  }
  AiUnit_PerformAction(
      bestX, bestY, AI_ACTION_REPAIR,
      Tile_GetDirection(bestX, bestY, bestUnit->x, bestUnit->y), 0);
}

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

  RangeMap_FillMoves(MAP_STATE->actingUnit->x, MAP_STATE->actingUnit->y,
                     MAP_STATE->actingUnit->type, UNLIMITED_MOVE, 1);
  RangeMap_MarkAdjacentTiles(111);
  groundX = NO_TILE_POS;
  fuelX = NO_TILE_POS;
  shipX = NO_TILE_POS;
  closestGround = 255;
  closestShip = closestGround;
  lowestFuel = 100;
  for (y = 0; y < MAP_STATE->unk_1c4->mapHeight; y++) {
    for (x = 0; x < MAP_STATE->unk_1c4->mapWidth; x++) {
      if (MAP_STATE->rangeMap[y][x] < 0) {
        continue;
      }
      tile = MAP_TILE_INDEX(x, y);
      unit = &MAP_STATE->units[MAP_TILES->unitAt[tile]];
      if (unit->type == UNIT_NONE || unit->type == UNIT_OOZIUM) {
        continue;
      }
      if (!Unit_IsCurrentPlayers(MAP_TILES->unk_508[tile])) {
        continue;
      }
      stats = &Ov000_gUnitStats[unit->type];
      if (stats->moveGroup & MOVE_GROUP_SEA) {
        if (unit->hp <= 90 && MAP_STATE->rangeMap[y][x] < closestShip) {
          closestShip = MAP_STATE->rangeMap[y][x];
          shipX = x;
          shipY = y;
        }
        if (unit->type == UNIT_LANDER || unit->type == UNIT_BLACK_BOAT) {
          continue;
        }
        if (MAP_STATE->unk_04 ==
            (MAP_TILES->tiles[MAP_TILE_INDEX(x, y)] & 0xe0)) {
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
        if (unit->hp <= 90 && MAP_STATE->rangeMap[y][x] < closestGround) {
          closestGround = MAP_STATE->rangeMap[y][x];
          groundX = x;
          groundY = y;
        }
      }
    }
  }
  if (shipX != NO_TILE_POS) {
    AiUnit_MoveTowardTile(shipX, shipY);
  }
  if (fuelX != NO_TILE_POS) {
    AiUnit_MoveTowardTile(fuelX, fuelY);
  }
  if (groundX != NO_TILE_POS) {
    AiUnit_MoveTowardTile(groundX, groundY);
  }
}

/* Moves the acting sea transport: one routine when it carries a unit,
 * another when it is empty. */
void AiSeaTransport_Move(void) {
  if (MAP_STATE->actingUnit->cargo[0] != 0) {
    AiSeaTransport_MoveLoaded();
  } else {
    AiSeaTransport_GoToFootUnit();
  }
}
