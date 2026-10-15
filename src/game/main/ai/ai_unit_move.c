#include "types.h"
#include "game/Unk_02080440.h"
#include "game/ai_unit_action.h"
#include "game/ai/ai_capture_site.h"
#include "game/ai/ai_service.h"
#include "game/map_object.h"
#include "game/combatant.h"
#include "game/math.h"
#include "game/ai_profile.h"
#include "game/terrain.h"
#include "game/unit_stats.h"
#include "game/ai/ai_threat.h"
#include "game/ai/ai_unit_claims.h"
#include "game/ai/ai_tile_search.h"
#include "game/ai/ai_move_search.h"
#include "game/map_unit.h"
#include "game/range_map.h"
#include "game/ai/ai_unit_move.h"
#include "game/ai/ai_perform_action.h"
#include "game/ai/ai_enemy_within.h"

/* The functions are in reverse address order: mwccarm emits a file's
 * functions last to first. */

/* queues an action for the unit at (x, y) */
#define MAP_STATE (data_027c027c->unk_48[data_027c0284])
#define UNIT_TILE(unit) (MAP_TILES->tiles[MAP_TILE_INDEX((unit)->x, (unit)->y)])
#define UNLIMITED_MOVE 110
#define NO_TILE 0x7fff
#define WORST_DANGER 768

extern u8 gTerrainFactoryKind[];

void RangeMap_FillUnitMoves(MapUnit* unit);
int Unit_GetTerrainStars(int player, int unitType, int terrain);
void Combat_CalcDamageToTarget(int unitIndex, int target, s16* pos);
int Unit_BestDamageToTargetKind(MapUnit* unit, int target);
/* base damage percent against a target kind */
BOOL Map_IsTileVisible(s16 x, s16 y);
int AiAttack_Score(int arg, s16* pos);
/* 0: direct (max range 1), 1: indirect, 2: no weapon, 3: Black Bomb, 4: Oozium */
int UnitType_GetAttackKind(u8 unitType);
/* per terrain id: 1 base, 2 airport, 3 port, else 0 */
int GetUnitMaxRange(int player, int unitType);
int Unit_IsTileOwnedByTeam(int unitIndex, int tile);
/* 1: tile owned by the unit's team */
BOOL Map_IsOwnedNonRepairTile(int x, int y);
void RangeMap_MarkAdjacentTiles(int arg);
void func_020b81b0(void);
void RangeMap_FillMoves(int x, int y, int unitType, int movement, int arg4);
int Map_AxisDistanceGap(s16 x0, s16 y0, s16 x1, s16 y1);
BOOL Unit_IsEnemyTile(int unitIndex, s16 x, s16 y);

/* TRUE when the acting unit may end its move on (x, y) on the way to a
 * target (AiUnit_MoveTowardTile). The tile's threatMax and threatSum must
 * stay below the limits in gAiActiveUnitProfile, Ai_CanEndMoveAt must
 * allow it and Ai_IsTileNeededByAlly must not claim it. An armed unit
 * that does not move by sea may not stop on ports unless they are enemy-owned,
 * and a unit waiting for a T-Copter (transportWanted 2) may not stop on ports
 * and shoals. */
BOOL AiTile_IsSafeStop(s16 x, s16 y) {
  /* needed to match: the first read adds the scaled index first */
  int tile = x + STATE_IDX_FIRST->mapTiles->rowStart[y];

  if (MAP_STATE->mapTiles->threatMax[tile] >=
      gAiActiveUnitProfile->threatMaxLimit) {
    return FALSE;
  }
  if (MAP_STATE->mapTiles->threatSum[tile] >=
      gAiActiveUnitProfile->threatSumLimit) {
    return FALSE;
  }
  if (!Ai_CanEndMoveAt(x, y)) {
    return FALSE;
  }
  if (Ov000_gUnitStats[MAP_STATE->actingUnit->type].moveGroup !=
          MOVE_GROUP_SEA &&
      UnitType_GetAttackKind(MAP_STATE->actingUnit->type) != ATTACK_KIND_NONE &&
      MAP_TILE_TERRAIN(MAP_STATE->mapTiles->tiles[MAP_TILE_INDEX(x, y)]) ==
          TERRAIN_PORT) {
    if (!Unit_IsEnemyTile(MAP_STATE->actingUnitIndex, x, y)) {
      return FALSE;
    }
  }
  if (Ai_IsTileNeededByAlly(x, y) == 1) {
    return FALSE;
  }
  if (MAP_STATE->actingUnit->transportWanted == 2) {
    int terrain =
        MAP_TILE_TERRAIN(MAP_STATE->mapTiles->tiles[MAP_TILE_INDEX(x, y)]);
    if (terrain == TERRAIN_PORT || terrain == TERRAIN_SHOAL) {
      return FALSE;
    }
  }
  return TRUE;
}

/* Moves the acting unit as far toward (targetX, targetY) as it can this
 * turn and waits there. MapTiles.rangeScratch gets the acting unit type's move
 * cost from the target. Of the tiles in reach (AiTile_IsSafeStop accepts them)
 * it takes the one with the lowest cost, then the lowest danger
 * (MapTiles.threatMax plus twice threatSum), then the shortest
 * Map_AxisDistanceGap distance to the target. A Sub or Stealth first tries the
 * same pick among tiles that are not enemy-owned and have no visible
 * enemy next to them; if one is found the unit goes there. */
void AiUnit_MoveTowardTile(int targetX, int targetY) {
  int prefCost;
  int bestX;
  int bestY;
  int prefX;
  int prefY;
  int bestDist;
  int prefDist;
  int bestCost;
  int bestDanger;
  int prefDanger;
  int cost;
  int danger;
  int dist;
  int x;
  int y;

  bestY = 0;
  bestX = -1;
  prefX = bestX;
  prefY = bestY;
  bestDist = bestY;
  prefDist = bestY;
  bestDanger = WORST_DANGER;
  prefDanger = bestDanger;
  RangeMap_SetBuffer(MAP_TILES->rangeScratch);
  RangeMap_FillMoves(targetX, targetY, MAP_STATE->actingUnit->type,
                     UNLIMITED_MOVE, 0);
  RangeMap_SetBuffer(MAP_TILES->rangeMapTiles);
  RangeMap_FillUnitMoves(MAP_STATE->actingUnit);
  if (AiTile_IsSafeStop(MAP_STATE->actingUnit->x, MAP_STATE->actingUnit->y)) {
    bestCost = MAP_TILES->rangeScratch[MAP_TILE_INDEX(
        MAP_STATE->actingUnit->x, MAP_STATE->actingUnit->y)];
  } else {
    bestCost = NO_TILE;
  }
  prefCost = bestCost;
  for (y = 0; y < MAP_STATE->unk_1c4->mapHeight; y++) {
    for (x = 0; x < MAP_STATE->unk_1c4->mapWidth; x++) {
      if (MAP_STATE->rangeMap[y][x] < 0) {
        continue;
      }
      if (MAP_TILES->rangeScratch[MAP_TILE_INDEX(x, y)] > bestCost) {
        continue;
      }
      if (!AiTile_IsSafeStop(x, y)) {
        continue;
      }
      danger = MAP_TILES->threatMax[MAP_TILE_INDEX(x, y)] +
               MAP_TILES->threatSum[MAP_TILE_INDEX(x, y)] * 2;
      dist = Map_AxisDistanceGap(targetX, targetY, x, y);
      if (MAP_STATE->actingUnit->type == UNIT_SUB ||
          MAP_STATE->actingUnit->type == UNIT_STEALTH) {
        if (Unit_IsEnemyTile(MAP_STATE->actingUnitIndex, x, y) != 1 &&
            Ai_HasVisibleEnemyWithin(x, y, 1) != 1) {
          cost = MAP_TILES->rangeScratch[MAP_TILE_INDEX(x, y)];
          if (prefCost == cost) {
            if (danger > prefDanger) {
              goto pickBest;
            }
            if (danger == prefDanger && dist > prefDist) {
              goto pickBest;
            }
          }
          prefCost = cost;
          prefX = x;
          prefY = y;
          prefDanger = danger;
          prefDist = dist;
        }
      }
    pickBest:
      cost = MAP_TILES->rangeScratch[MAP_TILE_INDEX(x, y)];
      if (bestCost == cost) {
        if (danger > bestDanger) {
          continue;
        }
        if (danger == bestDanger && dist > bestDist) {
          continue;
        }
      }
      bestCost = cost;
      bestDanger = danger;
      bestDist = dist;
      bestX = x;
      bestY = y;
    }
  }
  if (prefX > -1) {
    AiUnit_PerformAction(prefX, prefY, AI_ACTION_WAIT, 0, 0);
  }
  if (bestX <= -1) {
    return;
  }
  AiUnit_PerformAction(bestX, bestY, AI_ACTION_WAIT, 0, 0);
}

/* For a unit that does at least 10 damage to target kind 4: picks the
 * reachable tile with the lowest AiTile_GetLowestAdjacentReach score that holds a tile
 * with hp or a map object (as gAiTurnState->flags allows), and, if
 * AiUnit_FindFiringTile accepts it, moves toward it with AiUnit_MoveTowardTile. */
void AiUnit_HeadForObjectTarget(void) {
  int bestScore = 0x7fff;
  s16 pos[2];
  int score;
  int x;
  int y;

  if (Unit_BestDamageToTargetKind(MAP_STATE->actingUnit, 4) < 10) {
    return;
  }
  RangeMap_FillMoves(MAP_STATE->actingUnit->x, MAP_STATE->actingUnit->y,
                     MAP_STATE->actingUnit->type, 110, 1);
  RangeMap_MarkAdjacentTiles(111);
  for (y = 0; y < MAP_STATE->unk_1c4->mapHeight; y++) {
    for (x = 0; x < MAP_STATE->unk_1c4->mapWidth; x++) {
      if (MAP_STATE->rangeMap[y][x] < 0) {
        continue;
      }
      if ((MAP_STATE->tileHp[MAP_TILE_INDEX(x, y)] & 0x7f) != 0) {
        if (!AI_TARGET_FLAGS->attackTiles) {
          continue;
        }
      } else if (!AI_TARGET_FLAGS->attackObjects ||
                 Map_GetObjectAt(x, y) == NULL) {
        continue;
      }
      score = AiTile_GetLowestAdjacentReach(x, y);
      if (score < bestScore) {
        bestScore = score;
        pos[0] = x;
        pos[1] = y;
      }
    }
  }
  if (bestScore == 0x7fff) {
    return;
  }
  if (!AiUnit_FindFiringTile(pos)) {
    return;
  }
  AiUnit_MoveTowardTile(pos[0], pos[1]);
}

/* Last step of the AI order routines: the acting unit stays on its tile.
 * It first tries MoveToSafestTile with terrain ignored. */
void AiUnit_WaitInPlace(void) {
  gAiTurnState->stepFlags |= AI_STEP_IGNORE_TERRAIN;
  AiUnit_RetreatToRepair();
  AiUnit_MoveToSafestTile();
  func_020b81b0();
  Ai_ClearActingUnitTarget();
  AiUnit_PerformAction(MAP_STATE->actingUnit->x, MAP_STATE->actingUnit->y, 2, 0,
                       0);
}

/* Picks what the AI's acting direct-attack unit hits this turn and queues it:
 * the best-scored enemy unit (action 4) and an object or hp tile, preferring
 * low hp (action 5). Called from the AI unit actions through
 * AiUnit_PlanAttack when UnitType_GetAttackKind gives 0. */
void AiUnit_PickDirectAttack(void) {
  /* needed to match: declaration order sets the stack slots */
  int maxY;
  int minX;
  int unitAttackX;
  int unitAttackY;
  int unitScore;
  int onTargetSide;
  int holdsLastSide;
  int unitTargetSlot;
  int objectAttackX;
  int objectAttackY;
  int objectHp;
  int flags;
  int x;
  int y;
  s16 objectX;
  s16 objectY;
  int hp;
  int targetKind;
  int rating; /* needed to match: damage to an object, then the score of a unit attack */
  int aiBehaviour;
  int tileFlag;
  MapObject* object;
  MapUnit* target;
  MapUnit* neighbour;
  s16 pos[2];
  int maxX;
  int minY;
  int openSides;
  int xx;
  int yy;
  int dist;
  int tile;

  unitAttackX = -1;
  unitScore = 0;
  unitAttackY = -1;
  holdsLastSide = 0;
  unitTargetSlot = 0;
  objectAttackX = -1;
  objectAttackY = -1;
  objectHp = 255;
  if (MAP_STATE->unk_1c4->mapId == 238) {
    flags = 0;
  } else if (AiUnit_CruiserCanReachSub() == 1) {
    flags = AI_ATTACK_NO_CHIP;
  } else {
    flags = 0;
  }
  RangeMap_FillUnitMoves(MAP_STATE->actingUnit);
  RangeMap_MarkAdjacentTiles(111);
  for (y = 0; y < MAP_STATE->unk_1c4->mapHeight; y++) {
    for (x = 0; x < MAP_STATE->unk_1c4->mapWidth; x++) {
      if (MAP_STATE->rangeMap[y][x] < 0) {
        continue;
      }
      if (AI_TARGET_FLAGS->attackObjects) {
        object = Map_GetObjectAt(x, y);
        if (object != NULL) {
          hp = object->hp;
          targetKind = object->kind;
          goto attackObject;
        }
      }
      if (!AI_TARGET_FLAGS->attackTiles) {
        goto attackUnit;
      }
      targetKind = 6;
      tile = MAP_TILE_INDEX(x, y);
      hp = MAP_STATE->tileHp[tile] & 0x7f;
      if (hp == 0 || MAP_STATE->mapTiles->unitAt[tile] != 0) {
        goto attackUnit;
      }
    attackObject:
      rating = HwDiv32(
          Unit_BestDamageToTargetKind(MAP_STATE->actingUnit, targetKind) *
              MAP_STATE->actingUnit->hp,
          100);
      /* skip unless it destroys the object or deals at least 10 */
      if (rating < hp && rating < 10) {
        continue;
      }
      if (!FindAttackTile(x, y, pos)) {
        continue;
      }
      Combat_CalcDamageToTarget(MAP_STATE->actingUnitIndex, targetKind, pos);
      if (MAP_STATE->combatants[0].weapon == COMBAT_WEAPON_NONE) {
        continue;
      }
      if (rating >= objectHp) {
        if (rating < hp) {
          continue;
        }
        if (objectHp > hp) {
          continue;
        }
      } else if (objectHp < hp) {
        continue;
      }
      aiBehaviour = MAP_STATE->players[MAP_STATE->currentPlayer].aiBehaviour;
      if (aiBehaviour == AI_BEHAVIOUR_DEFENSE && rating < hp &&
          MAP_STATE->actingUnit->type > 2) {
        tile = MAP_TILE_INDEX(pos[0], pos[1]);
        if (MAP_STATE->mapTiles->threatMax[tile] >=
                gAiActiveUnitProfile->threatMaxLimit ||
            MAP_STATE->mapTiles->threatSum[tile] >=
                gAiActiveUnitProfile->threatSumLimit) {
          continue;
        }
      }
      if (aiBehaviour == AI_BEHAVIOUR_GENERAL &&
          Ai_CountReachableAdjacentTiles(unitAttackX, unitAttackY) >= 2 &&
          MAP_STATE->actingUnit->type > 2) {
        if (MAP_STATE->mapTiles->threatSum[MAP_TILE_INDEX(pos[0], pos[1])] >=
            gAiActiveUnitProfile->threatSumLimit) {
          continue;
        }
      }
      if (!Ai_CanEndMoveAt(pos[0], pos[1])) {
        continue;
      }
      objectHp = hp;
      objectAttackX = pos[0];
      objectAttackY = pos[1];
      objectX = x;
      objectY = y;
      continue;

    attackUnit:
      target =
          &MAP_STATE->units[MAP_STATE->mapTiles->unitAt[MAP_TILE_INDEX(x, y)]];
      if (target->type == 0) {
        continue;
      }
      if (MAP_STATE->players[(MAP_STATE->actingUnitIndex >> 6) + 1].team ==
          MAP_STATE->players[MAP_UNIT_OWNER(MAP_STATE, target)].team) {
        continue;
      }
      if (!Map_IsTileVisible(x, y)) {
        continue;
      }
      if (!FindAttackTile(x, y, pos)) {
        continue;
      }
      tileFlag = 0;
      tile = MAP_TILE_INDEX(pos[0], pos[1]);
      if (MAP_STATE->mapTiles->threatSum[tile] >=
              gAiActiveUnitProfile->threatSumLimit ||
          (MAP_STATE->mapTiles->threatMax[tile] >=
               gAiActiveUnitProfile->threatMaxLimit &&
           target->cargo[0] == 0 &&
           UnitType_GetAttackKind(target->type) == 2)) {
        if (!AiTile_HasStrongAllyInReach(pos[0], pos[1], target)) {
          tileFlag = AI_ATTACK_COST_VALUE;
        }
      }
      rating = AiAttack_Score(
          flags |
              (tileFlag | MAP_STATE->mapTiles->unitAt[MAP_TILE_INDEX(x, y)]),
          pos);
      if (rating == 0 || rating < unitScore) {
        continue;
      }
      if (AiUnit_TeammateOutdamages(target) == 1) {
        openSides = 0;
        onTargetSide = 0;
        minX = target->x - 1;
        if (minX < 0) {
          minX = 0;
        }
        minY = target->y - 1;
        if (minY < 0) {
          minY = 0;
        }
        maxX = MAP_STATE->unk_1c4->mapWidth - 1;
        if (target->x + 1 < maxX) {
          maxX = target->x + 1;
        }
        maxY = MAP_STATE->unk_1c4->mapHeight - 1;
        if (target->y + 1 < maxY) {
          maxY = target->y + 1;
        }
        for (yy = minY; yy <= maxY; yy++) {
          for (xx = minX; xx <= maxX; xx++) {
            /* tiles at distance 1: the target's four sides */
            dist = 0;
            if (target->x - xx > 0) {
              dist += target->x - xx;
            } else {
              dist += xx - target->x;
            }
            if (target->y - yy > 0) {
              dist += target->y - yy;
            } else {
              dist += yy - target->y;
            }
            if (dist < 1 || dist > 1) {
              continue;
            }
            if (MAP_STATE->rangeMap[yy][xx] < 0) {
              continue;
            }
            neighbour = &MAP_STATE->units[MAP_STATE->mapTiles
                                              ->unitAt[MAP_TILE_INDEX(xx, yy)]];
            if (neighbour->type != 0 && (neighbour->flags & MAP_UNIT_WAITED)) {
              continue;
            }
            openSides++;
            if (xx == MAP_STATE->actingUnit->x &&
                yy == MAP_STATE->actingUnit->y) {
              onTargetSide = 1;
            }
          }
        }
        /* one open side or none: leave the target, and note when the
                 * acting unit stands on that last side */
        if (openSides <= 1) {
          if (onTargetSide) {
            holdsLastSide = 1;
          }
          continue;
        }
      }
      unitScore = rating;
      unitAttackX = pos[0];
      unitAttackY = pos[1];
      unitTargetSlot = MAP_STATE->mapTiles->unitAt[MAP_TILE_INDEX(x, y)];
    }
  }
  if (unitScore != 0) {
    Ai_ClearActingUnitTarget();
    AiUnit_PerformAction(unitAttackX, unitAttackY, 4, unitTargetSlot, 0);
  }
  if (objectHp != 255) {
    Ai_ClearActingUnitTarget();
    AiUnit_PerformAction(objectAttackX, objectAttackY, 5, objectX, objectY);
  }
  if (holdsLastSide) {
    AiUnit_MoveToSafestTile();
  }
}

/* Plans the attack of the AI's acting unit, an indirect unit, from where it
 * stands. Every tile in range is tried: a map object or a tile with hp when
 * gAiTurnState allows, otherwise an enemy unit there. The best unit (by
 * AiAttack_Score's score) and the best tile are each queued with
 * AiUnit_PerformAction; a tile is kept only if the damage reaches 10 or its hp. */
void Ai_PlanIndirectAttack(void) {
  int maxY;
  int hp;
  int bestUnitScore;
  int bestUnitSlot;
  int bestTileHp;
  int distance;
  int flags;
  int minY;
  int x;
  s16 pos[2];
  int y;
  int minX;
  int maxX;
  int score;
  s16 bestTileX;
  MapUnit* target;
  int damage;
  int kind;
  s16 bestTileY;
  MapObject* object;

  bestUnitScore = 0;
  bestUnitSlot = 0;
  flags = 0;
  bestTileHp = 0xff; /* none yet */
  if (MAP_STATE->actingUnit->aiOrder != 7) {
    /* AI_ATTACK_TANK_VALUE: unit above 80 hp on its team's base.
         * AI_ATTACK_HEAVY_HIT: on its team's base, airport or port, or a
         * ground unit at 70 hp or less not on a tile where Map_IsOwnedNonRepairTile holds. */
    if (Unit_IsTileOwnedByTeam(MAP_STATE->actingUnitIndex,
                               UNIT_TILE(MAP_STATE->actingUnit)) == 1 &&
        MAP_TILE_TERRAIN(UNIT_TILE(MAP_STATE->actingUnit)) == TERRAIN_BASE &&
        MAP_STATE->actingUnit->hp > 80) {
      flags |= AI_ATTACK_TANK_VALUE;
    } else if ((Unit_IsTileOwnedByTeam(MAP_STATE->actingUnitIndex,
                                       UNIT_TILE(MAP_STATE->actingUnit)) == 1 &&
                gTerrainFactoryKind[MAP_TILE_TERRAIN(
                    UNIT_TILE(MAP_STATE->actingUnit))] != 0) ||
               ((Ov000_gUnitStats[MAP_STATE->actingUnit->type].moveGroup &
                 (MOVE_GROUP_FOOT | MOVE_GROUP_TIRES | MOVE_GROUP_TREAD)) !=
                    0 &&
                MAP_STATE->actingUnit->hp <= 70 &&
                Map_IsOwnedNonRepairTile(MAP_STATE->actingUnit->x,
                                         MAP_STATE->actingUnit->y) == 0)) {
      flags |= AI_ATTACK_HEAVY_HIT;
    }
  }
  pos[0] = MAP_STATE->actingUnit->x;
  pos[1] = MAP_STATE->actingUnit->y;
  if (MAP_STATE->actingUnit->x - GetUnitMaxRange(MAP_STATE->currentPlayer,
                                                 MAP_STATE->actingUnit->type) <
      0) {
    minX = 0;
  } else {
    minX =
        MAP_STATE->actingUnit->x -
        GetUnitMaxRange(MAP_STATE->currentPlayer, MAP_STATE->actingUnit->type);
  }
  if (MAP_STATE->actingUnit->y - GetUnitMaxRange(MAP_STATE->currentPlayer,
                                                 MAP_STATE->actingUnit->type) <
      0) {
    minY = 0;
  } else {
    minY =
        MAP_STATE->actingUnit->y -
        GetUnitMaxRange(MAP_STATE->currentPlayer, MAP_STATE->actingUnit->type);
  }
  maxX = MAP_STATE->unk_1c4->mapWidth - 1;
  if (MAP_STATE->actingUnit->x + GetUnitMaxRange(MAP_STATE->currentPlayer,
                                                 MAP_STATE->actingUnit->type) <
      maxX) {
    maxX =
        MAP_STATE->actingUnit->x +
        GetUnitMaxRange(MAP_STATE->currentPlayer, MAP_STATE->actingUnit->type);
  }
  maxY = MAP_STATE->unk_1c4->mapHeight - 1;
  if (MAP_STATE->actingUnit->y + GetUnitMaxRange(MAP_STATE->currentPlayer,
                                                 MAP_STATE->actingUnit->type) <
      maxY) {
    maxY =
        MAP_STATE->actingUnit->y +
        GetUnitMaxRange(MAP_STATE->currentPlayer, MAP_STATE->actingUnit->type);
  }
  for (y = minY; y <= maxY; y++) {
    for (x = minX; x <= maxX; x++) {
      distance = 0;
      if (MAP_STATE->actingUnit->x - x > 0) {
        distance += MAP_STATE->actingUnit->x - x;
      } else {
        distance += x - MAP_STATE->actingUnit->x;
      }
      if (MAP_STATE->actingUnit->y - y > 0) {
        distance += MAP_STATE->actingUnit->y - y;
      } else {
        distance += y - MAP_STATE->actingUnit->y;
      }
      if (distance < Ov000_gUnitStats[MAP_STATE->actingUnit->type].minRange) {
        continue;
      }
      if (distance > GetUnitMaxRange(MAP_STATE->currentPlayer,
                                     MAP_STATE->actingUnit->type)) {
        continue;
      }
      if (AI_TARGET_FLAGS->attackObjects) {
        object = Map_GetObjectAt(x, y);
        if (object != NULL) {
          hp = object->hp;
          kind = object->kind;
          goto attack_tile;
        }
      }
      if (AI_TARGET_FLAGS->attackTiles) {
        kind = MAP_OBJECT_TILE;
        hp = MAP_STATE->tileHp[MAP_TILE_INDEX(x, y)] & 0x7f;
        if (hp != 0 && MAP_TILES->unitAt[MAP_TILE_INDEX(x, y)] == 0) {
          goto attack_tile;
        }
      }
      goto attack_unit;
    attack_tile:
      damage =
          HwDiv32(Unit_BestDamageToTargetKind(MAP_STATE->actingUnit, kind) *
                      MAP_STATE->actingUnit->hp,
                  100);
      if (damage < hp && damage < 10) {
        continue;
      }
      Combat_CalcDamageToTarget(MAP_STATE->actingUnitIndex, kind, pos);
      if (MAP_STATE->combatants[0].weapon == COMBAT_WEAPON_NONE) {
        continue;
      }
      /* order between tiles; meaning unclear */
      if (damage >= bestTileHp) {
        if (damage < hp || bestTileHp > hp) {
          continue;
        }
      } else if (bestTileHp < hp) {
        continue;
      }
      if (MAP_STATE->players[MAP_STATE->currentPlayer].aiBehaviour ==
              AI_BEHAVIOUR_DEFENSE &&
          damage < hp) {
        if (MAP_TILES->threatMax[MAP_TILE_INDEX(pos[0], pos[1])] >=
            gAiActiveUnitProfile->threatMaxLimit) {
          continue;
        }
        if (MAP_TILES->threatSum[MAP_TILE_INDEX(pos[0], pos[1])] >=
            gAiActiveUnitProfile->threatSumLimit) {
          continue;
        }
      }
      if (Ai_CanEndMoveAt(pos[0], pos[1]) == 0) {
        continue;
      }
      bestTileX = x;
      bestTileY = y;
      bestTileHp = hp;
      continue;
    attack_unit:
      target = &MAP_STATE->units[MAP_TILES->unitAt[MAP_TILE_INDEX(x, y)]];
      if (target->type == UNIT_NONE) {
        continue;
      }
      if (MAP_STATE->players[(MAP_STATE->actingUnitIndex >> 6) + 1].team ==
          MAP_STATE->players[MAP_UNIT_OWNER(MAP_STATE, target)].team) {
        continue;
      }
      if (Map_IsTileVisible(x, y) == 0) {
        continue;
      }
      score = AiAttack_Score(
          flags | MAP_TILES->unitAt[MAP_TILE_INDEX(target->x, target->y)], pos);
      if (score != 0 && score >= bestUnitScore) {
        bestUnitScore = score;
        bestUnitSlot = MAP_TILES->unitAt[MAP_TILE_INDEX(x, y)];
      }
    }
  }
  if (bestUnitScore != 0) {
    Ai_ClearActingUnitTarget();
    AiUnit_PerformAction(MAP_STATE->actingUnit->x, MAP_STATE->actingUnit->y,
                         AI_ACTION_ATTACK_UNIT, bestUnitSlot, 0);
  }
  if (bestTileHp == 0xff) {
    return;
  }
  Ai_ClearActingUnitTarget();
  AiUnit_PerformAction(MAP_STATE->actingUnit->x, MAP_STATE->actingUnit->y,
                       AI_ACTION_ATTACK_TILE, bestTileX, bestTileY);
}

/* Queues the acting unit's attack, if it has one: direct units pick a
 * target next to them, indirect units one in range. */
void AiUnit_PlanAttack(void) {
  switch (UnitType_GetAttackKind(
      data_027c027c->unk_48[data_027c0284]->actingUnit->type)) {
    case 0:
      AiUnit_PickDirectAttack();
      break;
    case 1:
      Ai_PlanIndirectAttack();
      break;
  }
}

/* Attacks from where the acting unit stands, without moving. Like
 * AiUnit_PickDirectAttack, but looks only at the four neighbouring tiles
 * and omits the threat and "last open side" checks: queues an attack on the
 * best-scored visible enemy unit (action 4), and one on the object or hp
 * tile with the least hp (action 5). Used by AiFogDirect_ActOrder7. */
void AiUnit_AttackAdjacent(void) {
  /* needed to match: declaration order sets the stack slots and registers */
  int maxY;
  int maxX;
  int minX;
  int unitAttackX;
  int unitAttackY;
  int unitScore;
  int unitTargetSlot;
  int objectAttackX;
  int objectAttackY;
  s16 objectX;
  s16 objectY;
  int rating;
  MapObject* object;
  int minY;
  int hp;
  int objectHp;
  int targetKind;
  int y;
  int x;
  MapUnit* target;
  s16 pos[2];
  int dist;

  unitAttackX = -1;
  unitScore = 0;
  unitAttackY = -1;
  unitTargetSlot = 0;
  pos[0] = MAP_STATE->actingUnit->x;
  objectAttackX = -1;
  objectAttackY = -1;
  pos[1] = MAP_STATE->actingUnit->y;
  objectHp = 255;
  minX = MAP_STATE->actingUnit->x - 1;
  if (minX < 0) {
    minX = 0;
  }
  minY = MAP_STATE->actingUnit->y - 1;
  if (minY < 0) {
    minY = 0;
  }
  maxX = MAP_STATE->unk_1c4->mapWidth - 1;
  if (MAP_STATE->actingUnit->x + 1 < maxX) {
    maxX = MAP_STATE->actingUnit->x + 1;
  }
  maxY = MAP_STATE->unk_1c4->mapHeight - 1;
  if (MAP_STATE->actingUnit->y + 1 < maxY) {
    maxY = MAP_STATE->actingUnit->y + 1;
  }
  for (y = minY; y <= maxY; y++) {
    for (x = minX; x <= maxX; x++) {
      /* tiles at distance 1: the four sides of the unit */
      dist = 0;
      if (MAP_STATE->actingUnit->x - x > 0) {
        dist += MAP_STATE->actingUnit->x - x;
      } else {
        dist += x - MAP_STATE->actingUnit->x;
      }
      if (MAP_STATE->actingUnit->y - y > 0) {
        dist += MAP_STATE->actingUnit->y - y;
      } else {
        dist += y - MAP_STATE->actingUnit->y;
      }
      if (dist < 1 || dist > 1) {
        continue;
      }
      if (AI_TARGET_FLAGS->attackObjects) {
        object = Map_GetObjectAt(x, y);
        if (object != NULL) {
          hp = object->hp;
          targetKind = object->kind;
          goto attackObject;
        }
      }
      if (!AI_TARGET_FLAGS->attackTiles) {
        goto attackUnit;
      }
      targetKind = 6;
      hp = MAP_STATE->tileHp[MAP_TILE_INDEX(x, y)] & 0x7f;
      if (hp == 0 || MAP_STATE->mapTiles->unitAt[MAP_TILE_INDEX(x, y)] != 0) {
        goto attackUnit;
      }
    attackObject:
      rating = HwDiv32(
          Unit_BestDamageToTargetKind(MAP_STATE->actingUnit, targetKind) *
              MAP_STATE->actingUnit->hp,
          100);
      Combat_CalcDamageToTarget(MAP_STATE->actingUnitIndex, targetKind, pos);
      if (MAP_STATE->combatants[0].weapon == COMBAT_WEAPON_NONE) {
        continue;
      }
      if (rating >= objectHp) {
        if (rating < hp) {
          continue;
        }
        if (objectHp > hp) {
          continue;
        }
      } else if (objectHp < hp) {
        continue;
      }
      objectHp = hp;
      objectAttackX = pos[0];
      objectAttackY = pos[1];
      objectX = x;
      objectY = y;
      continue;

    attackUnit:
      target =
          &MAP_STATE->units[MAP_STATE->mapTiles->unitAt[MAP_TILE_INDEX(x, y)]];
      if (target->type == 0) {
        continue;
      }
      if (MAP_STATE->players[(MAP_STATE->actingUnitIndex >> 6) + 1].team ==
          MAP_STATE->players[MAP_UNIT_OWNER(MAP_STATE, target)].team) {
        continue;
      }
      if (!Map_IsTileVisible(x, y)) {
        continue;
      }
      rating = AiAttack_Score(MAP_STATE->mapTiles->unitAt[MAP_TILE_INDEX(x, y)],
                              pos);
      if (rating == 0 || rating < unitScore) {
        continue;
      }
      unitScore = rating;
      unitAttackX = pos[0];
      unitAttackY = pos[1];
      unitTargetSlot = MAP_STATE->mapTiles->unitAt[MAP_TILE_INDEX(x, y)];
    }
  }
  if (unitScore != 0) {
    Ai_ClearActingUnitTarget();
    AiUnit_PerformAction(unitAttackX, unitAttackY, 4, unitTargetSlot, 0);
  }
  if (objectHp == 255) {
    return;
  }
  Ai_ClearActingUnitTarget();
  AiUnit_PerformAction(objectAttackX, objectAttackY, 5, objectX, objectY);
}

/* Moves the acting unit to the free tile it can reach with the lowest
 * threat (MapTiles.threatMax + 2 * threatSum). Ties go to more terrain stars,
 * then to the lower rangeMap value. With AI_STEP_UNIT_AT_TARGET set, tiles
 * with a negative MapTiles.rangeScratch get -1 in rangeMap, and nothing is
 * queued when the best tile is the one the unit stands on. */
void AiUnit_MoveToSafestTile(void) {
  int bestX;
  int bestY;
  int bestThreat;
  int bestStars;
  int bestReach;
  int threat;
  int x;
  int y;
  int stars;
  int reach; /* rangeMap value of the tile */
  int tile;

  bestX = -1;
  bestY = bestX;
  bestThreat = 0x7fff;
  bestReach = bestThreat;
  bestStars = bestX;
  if (!(gAiTurnState->stepFlags & AI_STEP_IGNORE_TERRAIN)) {
    AiUnit_RetreatToRepair();
  }
  if ((gAiTurnState->stepFlags & AI_STEP_UNIT_AT_TARGET) &&
      (gAiTurnState->stepFlags & AI_STEP_STAY)) {
    return;
  }
  RangeMap_FillUnitMoves(MAP_STATE->actingUnit);
  for (y = 0; y < MAP_STATE->unk_1c4->mapHeight; y++) {
    for (x = 0; x < MAP_STATE->unk_1c4->mapWidth; x++) {
      if (MAP_STATE->rangeMap[y][x] < 0) {
        continue;
      }
      if (gAiTurnState->stepFlags & AI_STEP_UNIT_AT_TARGET) {
        if (MAP_STATE->mapTiles->rangeScratch[MAP_TILE_INDEX(x, y)] < 0) {
          MAP_STATE->rangeMap[y][x] = -1;
          continue;
        }
      }
      if (MAP_STATE->mapTiles->unitAt[MAP_TILE_INDEX(x, y)] != 0) {
        continue;
      }
      if (Ai_IsTileNeededByAlly(x, y) == 1) {
        continue;
      }
      if (!Ai_CanEndMoveAt(x, y)) {
        continue;
      }
      tile = MAP_TILE_INDEX(x, y);
      threat = MAP_STATE->mapTiles->threatMax[tile] +
               MAP_STATE->mapTiles->threatSum[tile] * 2;
      if (gAiTurnState->stepFlags & AI_STEP_IGNORE_TERRAIN) {
        stars = 0;
      } else {
        stars = Unit_GetTerrainStars(
            MAP_STATE->currentPlayer, MAP_STATE->actingUnit->type,
            MAP_TILE_TERRAIN(MAP_STATE->mapTiles->tiles[tile]));
      }
      reach = MAP_STATE->rangeMap[y][x];
      if (threat > bestThreat) {
        continue;
      }
      if (threat == bestThreat && stars < bestStars) {
        continue;
      }
      if (threat == bestThreat && stars == bestStars && reach > bestReach) {
        continue;
      }
      bestX = x;
      bestY = y;
      bestThreat = threat;
      bestStars = stars;
      bestReach = reach;
    }
  }
  if (bestX == -1) {
    return;
  }
  if ((gAiTurnState->stepFlags & AI_STEP_UNIT_AT_TARGET) &&
      bestX == MAP_STATE->actingUnit->x && bestY == MAP_STATE->actingUnit->y) {
    return;
  }
  Ai_ClearActingUnitTarget();
  AiUnit_PerformAction(bestX, bestY, 2, 0,
                       0); /* action 2: move and wait (guess) */
}
