#include "types.h"
#include "game/Unk_02080440.h"
#include "game/unit_stats.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;

typedef struct TilePos {
  s16 x;
  s16 y;
} TilePos;

int UnitType_GetAttackKind(int unitType);
BOOL Player_IsInGame(u8 player);
BOOL Unit_IsVisibleToCurrentPlayer(u16 slot);
/* Damage percent the attacker type deals to a dived unit, 0 if it cannot hit it. */
int Ai_BestDamageVsDived(int attacker, int target, int arg);
/* Best damage percent of the attacker type against the target type. */
int Ai_BestDamage(int attacker, int target, int arg);
int Player_CanSeeUnitAt(int player, int x, int y);
/* Checks the tile found and may move it; FALSE means the tile is not usable. */
BOOL AiUnit_FindFiringTile(s16* tile);
void AiActingUnit_MarkAttackRange(int movement);
int AiActingUnit_GetSearchMovement(void);

#define STATE (data_027c027c->unk_48[data_027c0284])

/* Looks for an enemy unit the acting unit can reach this turn and hurt for
 * at least 20 percent. Prefers the nearest one it hurts for 50 or more, else
 * the nearest of the weaker ones. A Cruiser counts a Sub as a quarter of the
 * distance. On success writes the unit's tile to tile and returns TRUE. */
BOOL AiUnit_FindEnemyTarget(s16* tile) {
  int player;
  int bestWeakCost;
  int score;
  int bestStrongCost;
  TilePos strong;
  int cost;
  int i;
  MapUnit* unit;
  TilePos weak;
  int y;
  int x;

  bestStrongCost = 0x7fff;
  bestWeakCost = 0x7fff;

  AiActingUnit_MarkAttackRange(AiActingUnit_GetSearchMovement());
  for (player = 1; player < ARMY_SLOT_COUNT; player++) {
    if (!Player_IsInGame(player) || STATE->players[STATE->currentPlayer].team ==
                                        STATE->players[player].team) {
      continue;
    }
    for (i = Ov000_gPlayerFirstUnitSlot[player] + 1;
         i < Ov000_gPlayerFirstUnitSlot[player] + 51; i++) {
      unit = &STATE->units[i];
      if (unit->type == UNIT_NONE || (unit->flags & MAP_UNIT_CARRIED) ||
          STATE->rangeMap[unit->y][unit->x] < 0) {
        continue;
      }
      if (UnitType_GetAttackKind(STATE->actingUnit->type) != 0 &&
          !Player_CanSeeUnitAt(STATE->currentPlayer, unit->x, unit->y)) {
        continue;
      }
      /* Subs and Stealths are hidden unless the player can see them; a Cruiser always finds a Sub. */
      if ((unit->type == UNIT_SUB && STATE->actingUnit->type != UNIT_CRUISER) ||
          unit->type == UNIT_STEALTH) {
        if (!Unit_IsVisibleToCurrentPlayer(i)) {
          continue;
        }
      }
      if ((unit->flags & MAP_UNIT_DIVED) &&
          !Ai_BestDamageVsDived(STATE->actingUnit->type, unit->type, 0)) {
        continue;
      }
      score = Ai_BestDamage(STATE->actingUnit->type, unit->type, 0);
      y = unit->y;
      x = unit->x;
      cost = STATE->rangeMap[y][x];
      if (STATE->actingUnit->type == UNIT_CRUISER && unit->type == UNIT_SUB) {
        cost >>= 2;
      }
      if (score < 20) {
        continue;
      }
      if (score < 50) {
        if (cost <= bestWeakCost) {
          bestWeakCost = cost;
          weak.x = unit->x;
          weak.y = unit->y;
        }
      } else if (cost <= bestStrongCost) {
        strong.x = unit->x;
        bestStrongCost = cost;
        strong.y = unit->y;
      }
    }
  }
  if (bestStrongCost == 0x7fff) {
    if (bestWeakCost == 0x7fff) {
      return 0;
    }
    strong = weak;
  }
  if (!AiUnit_FindFiringTile(&strong.x)) {
    return 0;
  }
  tile[0] = strong.x;
  tile[1] = strong.y;
  return 1;
}
