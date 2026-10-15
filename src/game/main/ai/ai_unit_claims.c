#include "types.h"
#include "game/Unk_02080440.h"
#include "game/unit_stats.h"
#include "game/range_map.h"
#include "game/ai_unit_action.h"
#include "game/map_unit.h"
#include "game/terrain.h"
#include "game/ai_profile.h"
#include "game/ai/ai_unit_claims.h"

/* The functions are in reverse address order: mwccarm emits a file's
 * functions last to first. */

/* RangeMap_FillMoves movement; guess: high enough to act as no limit. */
#define ANY_DISTANCE 110
/* the Unk_02080440_6088 entry of the property on a tile */
#define PROPERTY_AT(x, y) \
  (MAP_STATE->unk_6088[MAP_STATE->mapTiles->unk_1928[MAP_TILE_INDEX(x, y)]])

BOOL Player_IsInGame(u8 player);
void RangeMap_FillMoves(int x, int y, int unitType, s16 maxCost, int moveRange);
void RangeMap_MarkAdjacentTiles(int arg);
s16 AiActingUnit_GetSearchMovement(void);
BOOL Unit_IsCurrentPlayers(s16 unitSlot);
BOOL Terrain_IsPropertyNotSilo(u8 tile);
/* guess: the tile is a property other than a silo */
int UnitType_GetAttackKind(u8 type);
BOOL Map_IsSiloOrCapturableTile(int x, int y);
int Unit_GetUsableMovement(MapUnit* unit);
s16 AiUnit_GetSearchMovement(MapUnit* unit);
int Unit_CaptureCompletes(MapUnit* unit);
int UnitType_BestBaseDamage(int unitType, int targetType);

/* Whether a unit type is in the list kept for the terrain of a tile (base,
 * airport or port), 0 for any other terrain. A type that is not in the
 * airport list is checked against the port list. What the lists mean is not
 * known yet, so the name stays. */
int func_020a3dfc(int unitType, int tile) {
  switch (tile & 31) {
    case TERRAIN_BASE:
      switch (unitType) {
        case 3:
        case 4:
        case 5:
        case 6:
        case 8:
        case 9:
        case 10:
        case 11:
        case 12:
        case 14:
        case 17:
        case 19:
        case 21:
          return 1;
      }
      break;
    case TERRAIN_AIRPORT:
      switch (unitType) {
        case 9:
        case 12:
        case 14:
        case 15:
        case 16:
        case 25:
          return 1;
      }
    case TERRAIN_PORT:
      switch (unitType) {
        case 9:
        case 12:
        case 17:
        case 19:
        case 21:
        case 22:
        case 24:
        case 25:
          return 1;
      }
      break;
  }
  return 0;
}

/* TRUE when a same-team ally can reach (x, y) and hit the target: a direct
 * attacker that is not Infantry, Mech or Recon, with at least 60 hp, and
 * standing within the acting unit type's move range of (x, y) (the range
 * map is rebuilt around (x, y) in the scratch buffer rangeScratch and then
 * pointed back at rangeMapBuf). An ally that has already acted counts only
 * on a tile whose threatSum reaches the profile limit. */
BOOL AiTile_HasStrongAllyInReach(int x, int y, MapUnit* target) {
  MapUnit* ally;
  int col;
  int row;

  RangeMap_SetBuffer(MAP_TILES->rangeScratch);
  RangeMap_FillMoves(x, y, MAP_STATE->actingUnit->type, gAiTurnState->unk_05,
                     0);
  for (row = 0; row < MAP_STATE->unk_1c4->mapHeight; row++) {
    for (col = 0; col < MAP_STATE->unk_1c4->mapWidth; col++) {
      if (MAP_STATE->rangeMap[row][col] < 0) {
        continue;
      }
      ally = &MAP_STATE->units[MAP_TILES->unitAt[MAP_TILE_INDEX(col, row)]];
      if (ally->type == UNIT_NONE) {
        continue;
      }
      if (ally == MAP_STATE->actingUnit) {
        continue;
      }
      if (MAP_STATE->players[(MAP_STATE->actingUnitIndex >> 6) + 1].team !=
          MAP_STATE->players[MAP_UNIT_OWNER(MAP_STATE, ally)].team) {
        continue;
      }
      if (UnitType_BestBaseDamage(ally->type, target->type) == 0) {
        continue;
      }
      if (UnitType_GetAttackKind(ally->type) != ATTACK_KIND_DIRECT) {
        continue;
      }
      if (ally->type <= UNIT_MECH || ally->type == UNIT_RECON) {
        continue;
      }
      if (ally->hp < 60) {
        continue;
      }
      if (ally->flags & MAP_UNIT_WAITED) {
        if (MAP_TILES->threatSum[MAP_TILE_INDEX(col, row)] <
            gAiActiveUnitProfile->threatSumLimit) {
          continue;
        }
      }
      RangeMap_SetBuffer(MAP_TILES->rangeMapTiles);
      return TRUE;
    }
  }
  RangeMap_SetBuffer(MAP_TILES->rangeMapTiles);
  return FALSE;
}

/* Called by AiUnit_PerformAction. For a capture by a unit with more than
 * 60 hp that would take the property this turn, sets MapUnit.unk_0c_5. What
 * the flag does is unknown; the name is a guess. */
void AiUnit_MarkFinishingCapture(int action) {
  if (action != AI_ACTION_CAPTURE) {
    return;
  }
  if (MAP_STATE->actingUnit->hp < 61) {
    return;
  }
  if (Unit_CaptureCompletes(MAP_STATE->actingUnit) != TRUE) {
    return;
  }
  MAP_STATE->actingUnit->unk_0c_5 = 1;
}

/* Queue routine run once by func_0208e7dc, the counterpart of
 * AiTurn_ReleaseCaptureClaims. For each of the AI player's infantry and
 * mechs with MapUnit.unk_0c_5 set (see AiUnit_MarkFinishingCapture), picks
 * the nearest tile it can reach that is a silo or capturable and that no
 * other unit has claimed, claims it (Unk_02080440_6088.claimCount) and stores
 * it as the unit's AI target. If the tile is within the distance
 * Unit_GetUsableMovement gives, the flag is cleared; if not, MapUnit.transportWanted is
 * set to 1 (meaning unknown). Ties go to the later tile. */
void AiTurn_ClaimCaptureTargets(void) {
  int i;
  MapUnit* unit;
  int bestX;
  int bestY;
  int best;
  int x;
  int y;

  for (i = Ov000_gPlayerFirstUnitSlot[MAP_STATE->currentPlayer] + 1;
       i < Ov000_gPlayerFirstUnitSlot[MAP_STATE->currentPlayer] + 51; i++) {
    unit = &MAP_STATE->units[i];
    if (unit->type != UNIT_NONE && unit->type <= UNIT_MECH && unit->unk_0c_5) {
      RangeMap_FillMoves(unit->x, unit->y, unit->type,
                         AiUnit_GetSearchMovement(unit), 0);
      bestY = -1;
      bestX = -1;
      best = 0x7fff;
      for (y = 0; y < MAP_STATE->unk_1c4->mapHeight; y++) {
        for (x = 0; x < MAP_STATE->unk_1c4->mapWidth; x++) {
          if (MAP_STATE->rangeMap[y][x] < 0) {
            continue;
          }
          if (!Map_IsSiloOrCapturableTile(x, y)) {
            continue;
          }
          if (PROPERTY_AT(x, y).claimCount != 0) {
            continue;
          }
          if (best >= MAP_STATE->rangeMap[y][x]) {
            bestX = x;
            bestY = y;
            best = MAP_STATE->rangeMap[y][x];
          }
        }
      }
      if (best != 0x7fff) {
        PROPERTY_AT(bestX, bestY).claimCount++;
        unit->aiTargetX = bestX;
        unit->aiTargetY = bestY;
        if (best <= Unit_GetUsableMovement(unit)) {
          unit->unk_0c_5 = 0;
        } else {
          unit->transportWanted = 1;
        }
      }
    }
  }
}

/* Queue routine run once at the end of the AI's unit actions (set by
 * func_0208e77c). For each of the AI player's infantry and mechs with
 * MapUnit.unk_0c_5 set (see AiUnit_MarkFinishingCapture), takes back the
 * claim it put on its capture target (Unk_02080440_6088.claimCount, raised
 * by AiFoot_PickCaptureTarget), or on the tile it stands on when it has no
 * target. Then clears the target and the flag. */
void AiTurn_ReleaseCaptureClaims(void) {
  int i;
  MapUnit* unit;

  for (i = Ov000_gPlayerFirstUnitSlot[MAP_STATE->currentPlayer] + 1;
       i < Ov000_gPlayerFirstUnitSlot[MAP_STATE->currentPlayer] + 51; i++) {
    unit = &MAP_STATE->units[i];
    if (unit->type != UNIT_NONE && unit->type <= UNIT_MECH && unit->unk_0c_5) {
      if (unit->aiTargetX != MAP_UNIT_NO_TARGET) {
        if (PROPERTY_AT(unit->aiTargetX, unit->aiTargetY).claimCount != 0) {
          PROPERTY_AT(unit->aiTargetX, unit->aiTargetY).claimCount--;
        }
      } else {
        if (PROPERTY_AT(unit->x, unit->y).claimCount != 0) {
          PROPERTY_AT(unit->x, unit->y).claimCount--;
        }
      }
      unit->aiTargetX = MAP_UNIT_NO_TARGET;
      unit->aiTargetY = MAP_UNIT_NO_TARGET;
      unit->unk_0c_5 = 0;
    }
  }
}

/* Run by AiUnit_SeekSeaTransport for a direct ground unit, when the army
 * started with a ground factory and now has sea units or a port. Fills
 * the range map as a Tank standing on the unit's tile would, then counts
 * what is on that landmass: properties other than silos, own units that
 * have claimedLandmass set, and how many of those are Infantry or Mech. Returns
 * FALSE at once when an own base is among the properties.
 *
 * Sets claimedLandmass on the unit when the landmass has few such units for its
 * properties. Infantry and Mech: no Infantry or Mech with up to 3
 * properties, at most 1 with more. Other units: no marked unit with up to
 * 3 properties, no marked non-foot unit with up to 6, at most 1 above.
 * Returns whether claimedLandmass is set. */
BOOL AiUnit_ClaimLandmass(void) {
  /* needed to match: declaration order sets the registers */
  int units;
  int slot;
  int properties;
  int footUnits;
  int x;
  int y;
  MapUnit* unit;

  if (!(gAiTurnState->factoryDomainsOwn & AI_DOMAIN_GROUND)) {
    return FALSE;
  }
  if (!(gAiTurnState->domainsOwn & AI_DOMAIN_SEA)) {
    return FALSE;
  }
  if (!(Ov000_gUnitStats[MAP_STATE->actingUnit->type].moveGroup &
        (MOVE_GROUP_FOOT | MOVE_GROUP_TIRES | MOVE_GROUP_TREAD))) {
    return FALSE;
  }
  if (UnitType_GetAttackKind(MAP_STATE->actingUnit->type) !=
      ATTACK_KIND_DIRECT) {
    return FALSE;
  }
  if (MAP_STATE->actingUnit->claimedLandmass) {
    return FALSE;
  }
  RangeMap_FillMoves(MAP_STATE->actingUnit->x, MAP_STATE->actingUnit->y,
                     UNIT_TANK, ANY_DISTANCE, 0);
  properties = 0;
  footUnits = 0;
  units = 0;
  for (y = 0; y < MAP_STATE->unk_1c4->mapHeight; y++) {
    for (x = 0; x < MAP_STATE->unk_1c4->mapWidth; x++) {
      if (MAP_STATE->rangeMap[y][x] < 0) {
        continue;
      }
      if (Terrain_IsPropertyNotSilo(
              MAP_STATE->mapTiles->tiles[MAP_TILE_INDEX(x, y)])) {
        if (MAP_STATE->unk_04 ==
                (MAP_STATE->mapTiles->tiles[MAP_TILE_INDEX(x, y)] & 0xe0) &&
            MAP_TILE_TERRAIN(
                MAP_STATE->mapTiles->tiles[MAP_TILE_INDEX(x, y)]) ==
                TERRAIN_BASE) {
          return FALSE;
        }
        properties++;
      }
      slot = MAP_STATE->mapTiles->unitAt[MAP_TILE_INDEX(x, y)];
      if (slot == 0) {
        continue;
      }
      if (!Unit_IsCurrentPlayers(slot)) {
        continue;
      }
      unit = &MAP_STATE->units[slot];
      if (!unit->claimedLandmass) {
        continue;
      }
      units++;
      if (unit->type <= UNIT_MECH) {
        footUnits++;
      }
    }
  }
  if (properties == 0) {
    return FALSE;
  }
  if (MAP_STATE->actingUnit->type <= UNIT_MECH) {
    if (properties <= 3) {
      if (footUnits == 0) {
        MAP_STATE->actingUnit->claimedLandmass = 1;
      }
    } else if (footUnits <= 1) {
      MAP_STATE->actingUnit->claimedLandmass = 1;
    }
  } else if (properties <= 3) {
    if (units == 0) {
      MAP_STATE->actingUnit->claimedLandmass = 1;
    }
  } else if (properties <= 6) {
    if (units - footUnits == 0) {
      MAP_STATE->actingUnit->claimedLandmass = 1;
    }
  } else if (units - footUnits <= 1) {
    MAP_STATE->actingUnit->claimedLandmass = 1;
  }
  return MAP_STATE->actingUnit->claimedLandmass ? TRUE : FALSE;
}

/* TRUE when the acting unit is a Cruiser and it can reach or get next to an
 * enemy Sub this turn. The range map is pointed at MapTiles.rangeScratch while
 * it is filled, then back at MapTiles.rangeMapTiles. */
int AiUnit_CruiserCanReachSub(void) {
  int player;
  int i;
  MapUnit* acting;

  if (MAP_STATE->actingUnit->type != UNIT_CRUISER) {
    return 0;
  }
  RangeMap_SetBuffer(MAP_STATE->mapTiles->rangeScratch);
  acting = MAP_STATE->actingUnit;
  RangeMap_FillMoves(acting->x, acting->y, acting->type,
                     AiActingUnit_GetSearchMovement(), 1);
  RangeMap_MarkAdjacentTiles(111);
  for (player = 1; player < ARMY_SLOT_COUNT; player++) {
    if (Player_IsInGame(player) &&
        MAP_STATE->players[MAP_STATE->currentPlayer].team !=
            MAP_STATE->players[player].team) {
      for (i = Ov000_gPlayerFirstUnitSlot[player] + 1;
           i < Ov000_gPlayerFirstUnitSlot[player] + 51; i++) {
        MapUnit* unit = &MAP_STATE->units[i];

        if (unit->type != UNIT_NONE && unit->type == UNIT_SUB &&
            MAP_STATE->rangeMap[unit->y][unit->x] >= 0) {
          RangeMap_SetBuffer(MAP_STATE->mapTiles->rangeMapTiles);
          return 1;
        }
      }
    }
  }
  RangeMap_SetBuffer(MAP_STATE->mapTiles->rangeMapTiles);
  return 0;
}
