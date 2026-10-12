#include "types.h"
#include "game/Unk_02080440.h"
#include "game/ai_unit_action.h"
#include "game/strike_target.h"
#include "game/terrain.h"
#include "game/unit_stats.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;

int UnitType_GetAttackKind(int unitType); /* 0 direct, 1 indirect, 2 unarmed */
void RangeMap_FillUnitMoves(MapUnit* unit);
void RangeMap_MarkAdjacentTiles(int arg);
void AiUnit_PerformAction(s16 x, s16 y, int action, u8 arg3, u8 arg4);
BOOL Map_IsTileVisible(s16 x, s16 y);
void Ai_ClearActingUnitTarget(void);
int AiUnit_TeammateOutdamages(MapUnit* target);
BOOL FindAttackTile(int x, int y, s16* pos);
int AiAttack_Score(int arg, s16* pos);
int Unit_CaptureEndsNextTurn(MapUnit* unit);
int Unit_CapturePoints(
    MapUnit* unit); /* capture points the unit adds per turn */

#define STATE (data_027c027c->unk_48[data_027c0284])
#define TILE(x, y) ((x) + STATE->mapTiles->rowStart[y])

#define ATTACK_KIND_INDIRECT 1

/* Infantry and mechs. Among the tiles in reach, notes the free missile silo
 * with the lowest rangeMap value and picks the enemy unit with the best
 * AiAttack_Score. Vehicles are targets only when indirect, or for a mech with
 * ammo. Infantry and mechs are targets when their capture ends next turn, or
 * when they have over 30 hp, are capturing or have over 60 hp, and the acting
 * unit adds under 12 capture points a turn. When AiUnit_TeammateOutdamages holds for the
 * target, it also needs two or more open sides the acting unit can reach.
 * A silo with a strike site worth launching at is chosen over the attack. */
void AiFoot_PickAttackTarget(void) {
  int maxY;
  int minX;
  int attackX;
  int attackY;
  int siloX;
  int siloY;
  int siloRank;
  int bestScore;
  int score;
  int bestSlot;
  int x;
  int y;
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

  attackX = -1;
  bestScore = 0;
  attackY = -1;
  siloX = -1;
  siloY = -1;
  bestSlot = 0;
  siloRank = 110;
  RangeMap_FillUnitMoves(STATE->actingUnit);
  RangeMap_MarkAdjacentTiles(111);
  for (y = 0; y < STATE->unk_1c4->mapHeight; y++) {
    for (x = 0; x < STATE->unk_1c4->mapWidth; x++) {
      if (STATE->rangeMap[y][x] < 0) {
        continue;
      }
      tile = TILE(x, y);
      if (MAP_TILE_TERRAIN(STATE->mapTiles->tiles[tile]) == TERRAIN_SILO &&
          STATE->mapTiles->unitAt[tile] == 0) {
        if (STATE->rangeMap[y][x] < siloRank) {
          siloRank = STATE->rangeMap[y][x];
          siloX = x;
          siloY = y;
        }
      }
      target = &STATE->units[STATE->mapTiles->unitAt[tile]];
      if (target->type == UNIT_NONE) {
        continue;
      }
      if (STATE->players[(STATE->actingUnitIndex >> 6) + 1].team ==
          STATE->players[MAP_UNIT_OWNER(STATE, target)].team) {
        continue;
      }
      if (target->type > UNIT_MECH) {
        if (UnitType_GetAttackKind(target->type) != ATTACK_KIND_INDIRECT) {
          if (STATE->actingUnit->type == UNIT_INFANTRY) {
            continue;
          }
          if (STATE->actingUnit->ammo == 0) {
            continue;
          }
        }
      } else if (!Unit_CaptureEndsNextTurn(target)) {
        if (target->hp <= 30) {
          continue;
        }
        if (target->captureProgress == 0 && target->hp <= 60) {
          continue;
        }
        if (Unit_CapturePoints(STATE->actingUnit) >= 12) {
          continue;
        }
      }
      if (!Map_IsTileVisible(x, y)) {
        continue;
      }
      if (!FindAttackTile(x, y, pos)) {
        continue;
      }
      score = AiAttack_Score(STATE->mapTiles->unitAt[TILE(x, y)], pos);
      if (score == 0) {
        continue;
      }
      /* infantry against a vehicle: the target has to lose 15 hp or die */
      if (STATE->actingUnit->type == UNIT_INFANTRY &&
          target->type > UNIT_MECH && STATE->combatants[1].hp != 0 &&
          STATE->combatants[1].hpLost < 15) {
        continue;
      }
      if (score < bestScore) {
        continue;
      }
      if (AiUnit_TeammateOutdamages(target) == 1) {
        openSides = 0;
        minX = target->x - 1;
        if (minX < 0) {
          minX = 0;
        }
        minY = target->y - 1;
        if (minY < 0) {
          minY = 0;
        }
        maxX = STATE->unk_1c4->mapWidth - 1;
        if (target->x + 1 < maxX) {
          maxX = target->x + 1;
        }
        maxY = STATE->unk_1c4->mapHeight - 1;
        if (target->y + 1 < maxY) {
          maxY = target->y + 1;
        }
        for (yy = minY; yy <= maxY; yy++) {
          for (xx = minX; xx <= maxX; xx++) {
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
            /* the target's four sides */
            if (dist < 1 || dist > 1) {
              continue;
            }
            if (STATE->rangeMap[yy][xx] < 0) {
              continue;
            }
            neighbour = &STATE->units[STATE->mapTiles->unitAt[TILE(xx, yy)]];
            if (neighbour->type != UNIT_NONE &&
                (neighbour->flags & MAP_UNIT_WAITED)) {
              continue;
            }
            openSides++;
          }
        }
        if (openSides <= 1) {
          continue;
        }
      }
      bestScore = score;
      attackX = pos[0];
      attackY = pos[1];
      bestSlot = STATE->mapTiles->unitAt[TILE(x, y)];
    }
  }
  if (siloX != -1) {
    pos[0] = siloX;
    pos[1] = siloY;
    if (Strike_FindBestSite(pos, STATE->currentPlayer, 1,
                            STRIKE_MODE_AI_FUNDS) == 1) {
      AiUnit_PerformAction(siloX, siloY, AI_ACTION_LAUNCH_SILO, pos[0], pos[1]);
      return;
    }
  }
  if (bestScore == 0) {
    return;
  }
  Ai_ClearActingUnitTarget();
  AiUnit_PerformAction(attackX, attackY, AI_ACTION_ATTACK_UNIT, bestSlot, 0);
}
