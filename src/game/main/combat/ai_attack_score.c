#include "types.h"
#include "game/Unk_02080440.h"
#include "game/ai_unit_action.h"
#include "game/combatant.h"
#include "game/map_unit.h"
#include "game/unit_stats.h"
#include "game/terrain.h"
#include "game/math.h"

int UnitType_GetAttackKind(u8 unitType);
void Combat_SimulateChecked(
    int attackerIndex, int defenderIndex,
    s16* pos); /* Combat_SimulateBattle; halts when defenderIndex is 0 */
int UnitType_BestBaseDamage(
    int unitType,
    int targetType); /* best base damage of unitType against targetType, over both weapons */
int Unit_CanRepairAt(MapUnit* unit, int x, int y);
BOOL Unit_IsEnemyTile(
    int unitIndex, s16 x,
    s16 y); /* 1: tile (x, y) is owned by a team other than the unit's */
int Unit_CaptureEndsNextTurn(
    MapUnit* unit); /* 1: the unit's capture finishes next turn */
int AiAttack_CountTeamUnitsInReach(MapUnit* unit);

#define TILE_INDEX(x, y) (MAP_STATE->mapTiles->rowStart[y] + (x))
#define ATTACKER (MAP_STATE->combatants[0])
#define DEFENDER (MAP_STATE->combatants[1])
#define CURRENT_PLAYER (MAP_STATE->players[MAP_STATE->currentPlayer])

/* Scores an attack by the unit in slot MAP_STATE->actingUnitIndex on the unit in the
 * flags' low 16 bits, from pos. The battle is simulated with luck, then
 * the score is (damage * mult - loss * 2) / 8, clamped to 1-0xffff, where
 * damage and loss are the funds / 10 each side loses. Returns 0 when the
 * flags' conditions fail or the attack is a bad trade. */
int AiAttack_Score(u32 flags, s16* pos) {
  MapUnit* attacker;
  MapUnit* defender;
  int loss;
  int damage;
  int mult;
  int terrain;
  u32 cost;
  u32 score;

  mult = 1;
  Combat_SimulateChecked(MAP_STATE->actingUnitIndex,
                         flags & AI_ATTACK_DEFENDER_MASK, pos);
  if (ATTACKER.weapon == COMBAT_WEAPON_NONE) {
    /* no weapon can hit */
    return 0;
  }
  attacker = ATTACKER.unit;
  defender = DEFENDER.unit;
  if (flags & AI_ATTACK_SAFE) {
    if (ATTACKER.hpLost >= 10) {
      return 0;
    }
    if (ATTACKER.hp == 0) {
      return 0;
    }
  }
  if (flags & AI_ATTACK_HEAVY_HIT) {
    if (UnitType_GetAttackKind(defender->type) == 2) {
      return 0;
    }
    if (DEFENDER.hp != 0 && DEFENDER.hpLost < 30) {
      return 0;
    }
  }

  loss = HwDiv32(
      (attacker->hp - ATTACKER.hp) * Ov000_gUnitStats[attacker->type].costDiv10,
      100);
  damage = HwDiv32(
      (defender->hp - DEFENDER.hp) * Ov000_gUnitStats[defender->type].costDiv10,
      100);

  /* An enemy about to finish capturing an HQ skips every check below. */
  if (defender->captureProgress != 0 &&
      MAP_TILE_TERRAIN(
          MAP_STATE->mapTiles->tiles[TILE_INDEX(defender->x, defender->y)]) ==
          TERRAIN_HQ &&
      Unit_IsEnemyTile(
          MAP_STATE->mapTiles->unk_508[TILE_INDEX(attacker->x, attacker->y)],
          defender->x, defender->y) == 0 &&
      Unit_CaptureEndsNextTurn(defender) == 1) {
    goto bonuses;
  }

  /* needed to match: the (u32) casts below; the original compares unsigned */
  if (flags & AI_ATTACK_TANK_VALUE) {
    if (UnitType_GetAttackKind(defender->type) == 2) {
      return 0;
    }
    if ((u32)damage <
        HwDiv32(Ov000_gUnitStats[UNIT_TANK].costDiv10 * 60, 100)) {
      return 0;
    }
  }
  /* For vehicles other than Recon: target armed, damage worth 40% of a
     * Medium Tank (40% of a Tank when the attacker costs less than a Medium
     * Tank), target killed or loses 20 hp or more, and aiBehaviour not AI_BEHAVIOUR_DEFENSE. */
  if ((flags & AI_ATTACK_COST_VALUE) && attacker->type > UNIT_MECH &&
      attacker->type != UNIT_RECON) {
    if (UnitType_GetAttackKind(defender->type) == 2) {
      return 0;
    }
    if (Ov000_gUnitStats[UNIT_MEDIUM_TANK].costDiv10 <=
        Ov000_gUnitStats[attacker->type].costDiv10) {
      if ((u32)damage <
          HwDiv32(Ov000_gUnitStats[UNIT_MEDIUM_TANK].costDiv10 * 40, 100)) {
        return 0;
      }
    } else {
      if ((u32)damage <
          HwDiv32(Ov000_gUnitStats[UNIT_TANK].costDiv10 * 40, 100)) {
        return 0;
      }
    }
    if (DEFENDER.hp != 0 && DEFENDER.hpLost < 20) {
      return 0;
    }
    if (CURRENT_PLAYER.aiBehaviour == AI_BEHAVIOUR_DEFENSE) {
      return 0;
    }
  }
  if ((flags & AI_ATTACK_NO_CHIP) && defender->type != UNIT_SUB &&
      CURRENT_PLAYER.aiBehaviour != AI_BEHAVIOUR_STRIKE) {
    if (DEFENDER.hp != 0 && DEFENDER.hpLost < 20) {
      return 0;
    }
    if (UnitType_GetAttackKind(defender->type) == 2 &&
        (Ov000_gUnitStats[defender->type].moveGroup & MOVE_GROUP_SEA)) {
      return 0;
    }
  }

  if (defender->type == UNIT_OOZIUM) {
    if (DEFENDER.hp == 0) {
      return 0xffff;
    }
    if (UnitType_GetAttackKind(attacker->type) == 1 ||
        attacker->type <= UNIT_MECH) {
      return 1;
    }
    return CURRENT_PLAYER.aiBehaviour == AI_BEHAVIOUR_STRIKE ? 1 : -1;
  }
  /* Chip damage (10 hp or less) and costly trades count less. */
  if (defender->type != UNIT_MEGATANK) {
    if (DEFENDER.hp != 0 && defender->hp - DEFENDER.hp <= 10) {
      damage >>= 2;
    }
    if (attacker->hp - ATTACKER.hp >= 70 &&
        CURRENT_PLAYER.aiBehaviour != AI_BEHAVIOUR_STRIKE) {
      damage >>= 2;
    }
  } else {
    if (DEFENDER.hp != 0 && defender->hp - DEFENDER.hp <= 10 &&
        Unit_CanRepairAt(defender, defender->x, defender->y) == 1) {
      damage >>= 2;
    }
    if (defender->hp - DEFENDER.hp <= 10 && attacker->hp - ATTACKER.hp >= 70 &&
        CURRENT_PLAYER.aiBehaviour != AI_BEHAVIOUR_STRIKE) {
      damage >>= 1;
    }
  }

bonuses:
  if ((attacker->type == UNIT_FIGHTER && defender->type == UNIT_FIGHTER) ||
      (attacker->type == UNIT_BOMBER && defender->type == UNIT_CRUISER) ||
      (attacker->type == UNIT_BOMBER && defender->type == UNIT_ANTI_AIR)) {
    damage += damage >> 1;
    loss >>= 1;
  } else if (defender->type == UNIT_MEGATANK) {
    damage <<= 1;
  }
  if (DEFENDER.hp == 0 &&
      Unit_CanRepairAt(defender, defender->x, defender->y) == 1) {
    damage += HwDiv32(Ov000_gUnitStats[defender->type].costDiv10 * 20, 100);
  }
  /* Infantry and mech: worth more, and more again while capturing. */
  if ((u8)(defender->type + 0xff) <= 1) { /* needed to match: + 0xff, not - 1 */
    if (defender->type == UNIT_INFANTRY) {
      damage <<= 1;
    } else {
      damage = (damage * 3) >> 1;
    }
    if (defender->captureProgress != 0 &&
        Unit_IsEnemyTile(
            MAP_STATE->mapTiles->unk_508[TILE_INDEX(attacker->x, attacker->y)],
            defender->x, defender->y) != 1) {
      terrain = MAP_TILE_TERRAIN(
          MAP_STATE->mapTiles->tiles[TILE_INDEX(defender->x, defender->y)]);
      if (terrain == TERRAIN_HQ) {
        mult <<= 3;
      } else if (terrain == TERRAIN_BASE) {
        mult <<= 2;
      } else if (terrain == TERRAIN_AIRPORT || terrain == TERRAIN_PORT ||
                 terrain == TERRAIN_COM_TOWER) {
        mult <<= 1;
      }
      if (Unit_CaptureEndsNextTurn(defender) == 1) {
        if (DEFENDER.hp == 0 && defender->hp < 50) {
          damage = HwDiv32(damage * 50, defender->hp);
        }
        mult <<= 1;
        if (MAP_TILE_TERRAIN(MAP_STATE->mapTiles->tiles[TILE_INDEX(
                defender->x, defender->y)]) == TERRAIN_HQ) {
          mult <<= 1;
        }
      }
    }
  }

  /* Unarmed targets: worth a quarter when empty, half when carrying and
     * surviving. Black Bomb: a kill counts double, plus half its cost. */
  if (UnitType_GetAttackKind(defender->type) == 2) {
    if (defender->cargo[0] == 0) {
      damage >>= 2;
    } else if (DEFENDER.hp != 0) {
      damage >>= 1;
    }
  } else if (UnitType_GetAttackKind(defender->type) == 0) {
    mult <<= 1;
  } else if (UnitType_GetAttackKind(defender->type) == 1) {
    mult *= 3;
  } else if (DEFENDER.hp == 0) {
    if (DEFENDER.hp == 0 && defender->hp < 80) {
      damage = HwDiv32(damage * 80, defender->hp);
    }
    mult <<= 1;
    damage += HwDiv32(Ov000_gUnitStats[defender->type].costDiv10 * 50, 100);
  } else {
    damage >>= 1;
  }

  /* A losing trade is kept only when the target cannot hit back. */
  if (damage * mult - loss * 2 < 0) {
    if (UnitType_BestBaseDamage(defender->type, attacker->type) <= 1 &&
        ATTACKER.hp != 0) {
      loss = 0;
    } else {
      return 0;
    }
  }
  damage += (damage * AiAttack_CountTeamUnitsInReach(defender)) >> 5;
  if (MAP_STATE->mapTiles->threatMax[TILE_INDEX(pos[0], pos[1])] <= 50 &&
      MAP_STATE->mapTiles->threatSum[TILE_INDEX(pos[0], pos[1])] == 0 &&
      !(attacker->flags & MAP_UNIT_DIVED)) {
    cost = Ov000_gUnitStats[defender->type].costDiv10;
    if (cost < 800) {
      damage += cost;
    } else {
      damage += 800;
    }
  }
  score = (damage * mult - loss * 2) >> 3;
  if (score & 0xffff0000) {
    score = 0xffff;
  }
  if (score == 0) {
    score = 1;
  }
  return score;
}
