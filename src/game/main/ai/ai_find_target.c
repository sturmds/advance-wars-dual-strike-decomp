#include "types.h"
#include "game/Unk_02080440.h"
#include "game/Unk_02088d04.h"
#include "game/ai_unit_action.h"
#include "game/screen_other_map.h"
#include "game/ai/ai_tile_search.h"
#include "game/player.h"
#include "game/unit_stats.h"
#include "game/ai_profile.h"
#include "game/ai/ai_capture_site.h"
#include "game/map_unit.h"
#include "game/range_map.h"
#include "game/ai/ai_find_target.h"
#include "game/ai/ai_transport_act.h"
#include "game/ai/ai_unit_move.h"
#include "game/ai/ai_perform_action.h"
#include "game/ai/ai_unit_claims.h"
#include "game/ai/ai_enemy_within.h"
#include "game/ai.h"

/* The functions are in reverse address order: mwccarm emits a file's
 * functions last to first. */

/* The game state of the other map, one of the two in data_027c0288. */
#define OTHER_MAP ((&data_027c0288.unk_00)[1 - data_027c0284])
#define FAR_AWAY \
  110 /* movement of the fill, large enough to reach the whole map */
#define PLAYER (MAP_STATE->players[MAP_STATE->currentPlayer])
#define HQ_COORD_MASK \
  0x7f /* the low 7 bits of Player.hqX and Player.hqY hold the HQ tile; bit 0x80 of hqX means no HQ */
#define NO_TILE 0x7fff

extern Unk_02080440_48 data_02189cd8;
extern u8 gAiTransportsSentToUnit[];

void RangeMap_FillUnitMoves(MapUnit* unit);
BOOL Unit_IsTileOwnedByTeam(int unitIndex, int tile);
BOOL func_0209dd7c(void);
BOOL Player_HasHq(int player);
void RangeMap_FillMoves(int x, int y, int unitType, int movement, int arg4);
int UnitType_GetAttackKind(u8 unitType);
BOOL Terrain_IsProperty(u8 terrain);
BOOL Unit_IsCurrentPlayers(s16 unitSlot);
BOOL Map_IsCapturableByActingUnit(s16 x, s16 y);

/* Runs the two setup passes, then two tile searches in turn. Writes the
 * tile to out only when both succeed. */
BOOL func_020aac60(s16* out) {
  s16 tile[2];

  AiActingUnit_MarkAttackRange(AiActingUnit_GetSearchMovement());
  if (!Ai_FindNearestEnemyHqTile(tile)) {
    return FALSE;
  }
  if (!AiUnit_FindFiringTile(tile)) {
    return FALSE;
  }
  out[0] = tile[0];
  out[1] = tile[1];
  return TRUE;
}

/* Looks for a target tile (func_020aac60) and, if there is one, moves the
 * acting unit there (AiUnit_MoveTowardTile). */
void func_020aac28(void) {
  s16 tile[2];

  if (func_020aac60(tile)) {
    AiUnit_MoveTowardTile(tile[0], tile[1]);
  }
}

/* Finds a capture site (an entry of the unk_6088 table) for the acting
 * unit: the reachable tile with the lowest range map value that
 * Map_IsCapturableByActingUnit accepts, that Ai_HasEnemyWithinMoves accepts, and
 * whose site counter is under the AI profile's limit. The counter is
 * unk_04 for a unit that is not indirect and unk_05 for an indirect one.
 * Passes the tile to AiUnit_FindFiringTile and writes the result to out. */
BOOL Ai_FindCaptureSiteTile(s16* out) {
  int counterOffset;
  int best;
  int x;
  int y;
  s16 tile[2];

  best = NO_TILE;
  counterOffset = UnitType_GetAttackKind(MAP_STATE->actingUnit->type) ==
                          ATTACK_KIND_INDIRECT
                      ? 2
                      : 1;
  AiActingUnit_MarkAttackRange(AiActingUnit_GetSearchMovement());
  for (y = 0; y < MAP_STATE->unk_1c4->mapHeight; y++) {
    for (x = 0; x < MAP_STATE->unk_1c4->mapWidth; x++) {
      if (MAP_STATE->rangeMap[y][x] < 0) {
        continue;
      }
      if (!Map_IsCapturableByActingUnit(x, y)) {
        continue;
      }
      if ((&MAP_STATE->unk_6088[MAP_TILES->unk_1928[x + MAP_TILES->rowStart[y]]]
                .claimCount)[counterOffset] >= gAiProfile->unk_08[5]) {
        continue;
      }
      if (best < MAP_STATE->rangeMap[y][x]) {
        continue;
      }
      if (!Ai_HasEnemyWithinMoves(x, y, 1, 5)) {
        continue;
      }
      tile[0] = x;
      tile[1] = y;
      best = MAP_STATE->rangeMap[y][x];
    }
  }
  if (best == NO_TILE) {
    return FALSE;
  }
  if (!AiUnit_FindFiringTile(tile)) {
    return FALSE;
  }
  out[0] = tile[0];
  out[1] = tile[1];
  return TRUE;
}

/* Looks for a target tile (Ai_FindCaptureSiteTile) and, if there is one, moves the
 * acting unit there (AiUnit_MoveTowardTile). */
void func_020aa9c4(void) {
  s16 tile[2];

  if (Ai_FindCaptureSiteTile(tile)) {
    AiUnit_MoveTowardTile(tile[0], tile[1]);
  }
}

/* For a land transport: picks an own Infantry or Mech it can reach this turn
 * that no other transport has claimed (gAiTransportsSentToUnit), preferring
 * the one it already chose and foot units with an enemy within 5 tiles
 * (Ai_HasEnemyWithin). Claims the foot unit, stores its slot in the
 * transport's aiTargetX and writes the tile to move to. If the foot unit has a target of its own, the tile is the one
 * the transport reaches that is closest to that target. */
BOOL AiTransport_PickFootUnit(s16* tile) {
  MapUnit* unit;
  int col;
  int cost;
  int bestX;
  int row;
  int bestY;
  int bestCost;
  int slot;
  int x;
  int y;
  bestX = -1;
  bestY = -1;
  bestCost = 0x7fff;

  if (Ov000_gUnitStats[MAP_STATE->actingUnit->type].moveGroup &
      MOVE_GROUP_SEA) {
    return FALSE;
  }
  RangeMap_FillMoves(MAP_STATE->actingUnit->x, MAP_STATE->actingUnit->y,
                     MAP_STATE->actingUnit->type,
                     AiActingUnit_GetSearchMovement(), 0);
  for (y = 0; y < MAP_STATE->unk_1c4->mapHeight; y++) {
    for (x = 0; x < MAP_STATE->unk_1c4->mapWidth; x++) {
      if (MAP_STATE->rangeMap[y][x] < 0) {
        continue;
      }
      slot = MAP_STATE->mapTiles->unitAt[MAP_TILE_INDEX(x, y)];
      if (slot == 0 || !Unit_IsCurrentPlayers(slot)) {
        continue;
      }
      if (gAiTransportsSentToUnit[slot & 63] != 0) {
        continue;
      }
      unit = &MAP_STATE->units[slot];
      if (unit->type > UNIT_MECH) {
        continue;
      }
      cost = MAP_STATE->rangeMap[y][x];
      if ((slot & 63) != MAP_STATE->actingUnit->aiTargetX || !unit->unk_0c_6) {
        cost += 256;
      }
      if (!Ai_HasEnemyWithin(x, y, 5)) {
        cost += 512;
      }
      if (bestCost >= cost) {
        bestX = x;
        bestY = y;
        bestCost = cost;
      }
    }
  }
  if (bestCost == 0x7fff) {
    MAP_STATE->actingUnit->aiTargetX = MAP_UNIT_NO_TARGET;
    return FALSE;
  }
  gAiTransportsSentToUnit
      [MAP_STATE->mapTiles->unitAt[MAP_TILE_INDEX(bestX, bestY)] & 63]++;
  MAP_STATE->actingUnit->aiTargetX =
      MAP_STATE->mapTiles->unitAt[MAP_TILE_INDEX(bestX, bestY)] & 63;
  unit =
      &MAP_STATE
           ->units[MAP_STATE->mapTiles->unitAt[MAP_TILE_INDEX(bestX, bestY)]];
  unit->unk_0c_6 = 1;
  if (unit->aiTargetX != MAP_UNIT_NO_TARGET) {
    RangeMap_SetBuffer(MAP_STATE->mapTiles->rangeScratch);
    RangeMap_FillMoves(unit->aiTargetX, unit->aiTargetY,
                       MAP_STATE->actingUnit->type, 110, 0);
    RangeMap_SetBuffer(MAP_STATE->mapTiles->rangeMapTiles);
    RangeMap_FillMoves(bestX, bestY, MAP_STATE->actingUnit->type,
                       gAiTurnState->unk_05, 0);
    /* needed to match: the last search reuses bestCost */
    bestCost = MAP_STATE->mapTiles->rangeScratch[MAP_TILE_INDEX(bestX, bestY)];
    if (bestCost >= 0) {
      for (row = 0; row < MAP_STATE->unk_1c4->mapHeight; row++) {
        for (col = 0; col < MAP_STATE->unk_1c4->mapWidth; col++) {
          if (MAP_STATE->rangeMap[row][col] < 0) {
            continue;
          }
          cost = MAP_STATE->mapTiles->rangeScratch[MAP_TILE_INDEX(col, row)];
          if (bestCost >= cost) {
            bestX = col;
            bestY = row;
            bestCost = cost;
          }
        }
      }
    }
  }
  tile[0] = bestX;
  tile[1] = bestY;
  return TRUE;
}

/* Looks for a target tile (AiTransport_PickFootUnit) and, if there is one, moves the
 * acting unit there (AiUnit_MoveTowardTile). */
void func_020aa4a4(void) {
  s16 tile[2];

  if (AiTransport_PickFootUnit(tile)) {
    AiUnit_MoveTowardTile(tile[0], tile[1]);
  }
}

/* For a land or air unit: finds the nearest reachable property tile of the
 * current player that Ai_HasEnemyWithinMoves accepts, and writes its x and y to
 * out. An indirect unit skips tiles whose enemy threat sum has reached its
 * profile's limit. Returns FALSE when there is none. */
BOOL Ai_FindNearestOwnPropertyTile(s16* out) {
  int bestX = -1;
  int bestY = -1;
  int best = NO_TILE;
  int x;
  int y;

  if (Ov000_gUnitStats[MAP_STATE->actingUnit->type].moveGroup &
      MOVE_GROUP_SEA) {
    return FALSE;
  }
  RangeMap_FillMoves(MAP_STATE->actingUnit->x, MAP_STATE->actingUnit->y,
                     MAP_STATE->actingUnit->type, FAR_AWAY, 0);
  for (y = 0; y < MAP_STATE->unk_1c4->mapHeight; y++) {
    for (x = 0; x < MAP_STATE->unk_1c4->mapWidth; x++) {
      if (MAP_STATE->rangeMap[y][x] < 0) {
        continue;
      }
      if (!Terrain_IsProperty(MAP_TILE_TERRAIN(MAP_TILE_AT(x, y)))) {
        continue;
      }
      if (MAP_STATE->unk_04 != (MAP_TILE_AT(x, y) & 0xe0)) {
        continue;
      }
      if (best < MAP_STATE->rangeMap[y][x]) {
        continue;
      }
      if (!Ai_HasEnemyWithinMoves(x, y, 1, 5)) {
        continue;
      }
      if (UnitType_GetAttackKind(MAP_STATE->actingUnit->type) ==
          ATTACK_KIND_INDIRECT) {
        if (MAP_TILES->threatSum[MAP_TILE_INDEX(x, y)] >=
            gAiActiveUnitProfile->threatSumLimit) {
          continue;
        }
      }
      bestX = x;
      bestY = y;
      best = MAP_STATE->rangeMap[y][x];
    }
  }
  if (best == NO_TILE) {
    return FALSE;
  }
  out[0] = bestX;
  out[1] = bestY;
  return TRUE;
}

/* Looks for a target tile (Ai_FindNearestOwnPropertyTile) and, if there is one, moves the
 * acting unit there (AiUnit_MoveTowardTile). */
void func_020aa1ec(void) {
  s16 tile[2];

  if (Ai_FindNearestOwnPropertyTile(tile)) {
    AiUnit_MoveTowardTile(tile[0], tile[1]);
  }
}

/* Writes the x and y of the current player's HQ to out when the acting
 * unit (not a sea unit) can reach it. */
BOOL Ai_FindOwnHqTile(s16* out) {
  if (Ov000_gUnitStats[MAP_STATE->actingUnit->type].moveGroup &
      MOVE_GROUP_SEA) {
    return FALSE;
  }
  if (!Player_HasHq(MAP_STATE->currentPlayer)) {
    return FALSE;
  }
  RangeMap_FillMoves(MAP_STATE->actingUnit->x, MAP_STATE->actingUnit->y,
                     MAP_STATE->actingUnit->type, FAR_AWAY, 0);
  if (MAP_STATE->rangeMap[PLAYER.hqY & HQ_COORD_MASK]
                         [PLAYER.hqX & HQ_COORD_MASK] < 0) {
    return FALSE;
  }
  out[0] = PLAYER.hqX & HQ_COORD_MASK;
  out[1] = PLAYER.hqY & HQ_COORD_MASK;
  return TRUE;
}

/* Looks for a target tile (Ai_FindOwnHqTile) and, if there is one, moves the
 * acting unit there (AiUnit_MoveTowardTile). */
void func_020aa068(void) {
  s16 tile[2];

  if (Ai_FindOwnHqTile(tile)) {
    AiUnit_MoveTowardTile(tile[0], tile[1]);
  }
}

/* Runs the two setup passes, then takes the acting unit's AI target if
 * the range map reaches it and hands it to AiUnit_FindFiringTile. Writes the
 * tile that call leaves in tile to out when it succeeds. */
BOOL func_020a9f9c(s16* out) {
  s16 tile[2];

  AiActingUnit_MarkAttackRange(AiActingUnit_GetSearchMovement());
  if (MAP_STATE->actingUnit->aiTargetX == MAP_UNIT_NO_TARGET) {
    return FALSE;
  }
  if (MAP_STATE->rangeMap[MAP_STATE->actingUnit->aiTargetY]
                         [MAP_STATE->actingUnit->aiTargetX] < 0) {
    return FALSE;
  }
  tile[0] = MAP_STATE->actingUnit->aiTargetX;
  tile[1] = MAP_STATE->actingUnit->aiTargetY;
  if (!AiUnit_FindFiringTile(tile)) {
    return FALSE;
  }
  out[0] = tile[0];
  out[1] = tile[1];
  return TRUE;
}

/* Looks for a target tile (func_020a9f9c) and, if there is one, moves the
 * acting unit there (AiUnit_MoveTowardTile). */
void func_020a9f64(void) {
  s16 tile[2];

  if (func_020a9f9c(tile)) {
    AiUnit_MoveTowardTile(tile[0], tile[1]);
  }
}

/* Fills the range map with the acting unit's moves from its own tile. If
 * the unit has an AI target (MAP_UNIT_NO_TARGET means none) that the fill
 * reached, writes the target's x and y to out and returns TRUE. */
BOOL Ai_FindReachableAiTarget(s16* out) {
  RangeMap_FillMoves(MAP_STATE->actingUnit->x, MAP_STATE->actingUnit->y,
                     MAP_STATE->actingUnit->type, FAR_AWAY, 0);
  if (MAP_STATE->actingUnit->aiTargetX == MAP_UNIT_NO_TARGET) {
    return FALSE;
  }
  if (MAP_STATE->rangeMap[MAP_STATE->actingUnit->aiTargetY]
                         [MAP_STATE->actingUnit->aiTargetX] < 0) {
    return FALSE;
  }
  out[0] = MAP_STATE->actingUnit->aiTargetX;
  out[1] = MAP_STATE->actingUnit->aiTargetY;
  return TRUE;
}

/* Looks for a target tile (Ai_FindReachableAiTarget) and, if there is one, moves the
 * acting unit there (AiUnit_MoveTowardTile). */
void func_020a9e44(void) {
  s16 tile[2];

  if (Ai_FindReachableAiTarget(tile)) {
    AiUnit_MoveTowardTile(tile[0], tile[1]);
  }
}

/* Counterpart of AiUnit_ActByOrder: runs the target tile search of the
 * acting unit's AI order and writes the tile to site. FALSE: the search
 * found none. Order 7 gives the unit's own tile. The site is where
 * AiUnit_RetreatTowardSite heads for. */
BOOL AiUnit_PickOrderSite(s16* site) {
  switch (MAP_STATE->actingUnit->aiOrder) {
    case 0:
    case 1:
      if (!AiUnit_FindEnemyTarget(site)) {
        return FALSE;
      }
      break;
    case 2:
      if (func_0209dd7c() == TRUE) {
        if (!func_020aac60(site)) {
          return FALSE;
        }
      } else {
        if (!AiUnit_FindEnemyTarget(site)) {
          return FALSE;
        }
      }
      break;
    case 3:
      if (!Ai_FindCaptureSiteTile(site)) {
        return FALSE;
      }
      break;
    case 4:
      if (!AiTransport_PickFootUnit(site)) {
        return FALSE;
      }
      break;
    case 5:
      if (!Ai_FindNearestOwnPropertyTile(site)) {
        return FALSE;
      }
      break;
    case 6:
      if (Player_HasHq(MAP_STATE->currentPlayer) == TRUE) {
        if (!Ai_FindOwnHqTile(site)) {
          return FALSE;
        }
      } else {
        if (!Ai_FindNearestOwnPropertyTile(site)) {
          return FALSE;
        }
      }
      break;
    case 8:
    case 10:
      if (!func_020a9f9c(site)) {
        return FALSE;
      }
      break;
    case 9:
      if (!Ai_FindReachableAiTarget(site)) {
        return FALSE;
      }
      break;
    case 7:
      site[0] = MAP_STATE->actingUnit->x;
      site[1] = MAP_STATE->actingUnit->y;
      break;
    default:
      return FALSE;
  }
  return TRUE;
}

/* Moves the acting unit toward the reachable tile with the lowest rangeMap
 * value (the last one on a tie) whose owner is another team's and that
 * func_020a3dfc accepts for the unit type, if AiUnit_FindFiringTile agrees to the
 * tile in pos. AiActingUnit_MarkAttackRange runs first with the search movement. */
void AiUnit_MoveToEnemyProperty(void) {
  s16 pos[2];
  int y;
  int tile;
  int bestReach = 0x7fff;
  int x;

  AiActingUnit_MarkAttackRange(AiActingUnit_GetSearchMovement());
  for (y = 0; y < MAP_STATE->unk_1c4->mapHeight; y++) {
    for (x = 0; x < MAP_STATE->unk_1c4->mapWidth; x++) {
      if (MAP_STATE->rangeMap[y][x] < 0) {
        continue;
      }
      tile = MAP_STATE->mapTiles->tiles[MAP_TILE_INDEX(x, y)];
      if ((tile & 0xe0) == 0) {
        continue;
      }
      if (Unit_IsTileOwnedByTeam((s16)MAP_STATE->currentUnitBase, (u8)tile) ==
          1) {
        continue;
      }
      if (!func_020a3dfc(MAP_STATE->actingUnit->type, tile)) {
        continue;
      }
      if (bestReach < MAP_STATE->rangeMap[y][x]) {
        continue;
      }
      pos[0] = x;
      pos[1] = y;
      bestReach = MAP_STATE->rangeMap[y][x];
    }
  }
  if (bestReach == 0x7fff) {
    return;
  }
  if (!AiUnit_FindFiringTile(pos)) {
    return;
  }
  AiUnit_MoveTowardTile(pos[0], pos[1]);
}

/* Name is a guess from the checks. In a two-map battle, when the other
 * map's state has unk_30 clear and the acting unit may use it (claimedLandmass
 * clear), moves the unit to the reachable tile with the lowest rangeMap
 * value that AiUnit_CanUseOtherMapFrom accepts, with action 26. */
void AiUnit_MoveToOtherMap(void) {
  Unk_027c0280* saved0280;
  Unk_02080440* saved027c;
  u32 otherUnk30;
  s16 bestX;
  s16 bestY;
  int bestReach = 0x7fff;
  int x;
  int y;

  if (MAP_STATE->gameMode == GAME_MODE_CAMPAIGN) {
    return;
  }
  if (MAP_STATE->secondMapId == 0) {
    return;
  }
  saved0280 = data_027c0280;
  Screen_SetCurrentGfx(OTHER_MAP);
  saved027c = data_027c027c;
  Screen_SetCurrent(OTHER_MAP);
  otherUnk30 = MAP_STATE->unk_1c4->unk_30;
  Screen_SetCurrent(saved027c);
  Screen_SetCurrentGfx(saved0280);
  if (otherUnk30 != 0) {
    return;
  }
  if (MAP_STATE == &data_02189cd8) {
    return;
  }
  if (MAP_STATE->actingUnit->claimedLandmass) {
    return;
  }
  if (gAiTurnState->factoryDomainsOwn == 0) {
    return;
  }
  RangeMap_FillUnitMoves(MAP_STATE->actingUnit);
  bestX = -1;
  for (y = 0; y < MAP_STATE->unk_1c4->mapHeight; y++) {
    for (x = 0; x < MAP_STATE->unk_1c4->mapWidth; x++) {
      if (MAP_STATE->rangeMap[y][x] < 0) {
        continue;
      }
      if (AiUnit_CanUseOtherMapFrom(MAP_STATE->actingUnit, x, y) != 1) {
        continue;
      }
      if (MAP_STATE->rangeMap[y][x] <= bestReach) {
        bestY = y;
        bestReach = MAP_STATE->rangeMap[y][x];
        bestX = x;
      }
    }
  }
  if (bestX == -1) {
    return;
  }
  AiUnit_PerformAction(bestX, bestY, 26, 0, 0);
}
