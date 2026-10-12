#include "types.h"
#include "game/Unk_02080440.h"
#include "game/map_unit.h"
#include "game/unit_stats.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;

int abs(int x);
int UnitType_GetAttackKind(int unitType);
int Unit_GetUsableMovement(MapUnit* unit);
int Player_GetUnitMoveRange(int player, int unitType);
int GetUnitBaseMaxRange(int player, int unitType);

#define STATE (data_027c027c->unk_48[data_027c0284])

/* TRUE when the enemy unit is close enough to matter to the acting unit: the
 * tile distance is at most the acting unit's movement plus the enemy's move
 * range plus 1 (a direct attacker), or plus its max range (anything else), plus
 * slack. The unit in actingUnit adds 1 to slack when it carries cargo and cannot attack. */
BOOL Ai_EnemyWithinReach(MapUnit* acting, MapUnit* enemy, int slack) {
  int distance;
  int reach;

  if (STATE->actingUnit->cargo[0] != 0) {
    if (UnitType_GetAttackKind(STATE->actingUnit->type) == ATTACK_KIND_NONE) {
      slack++;
    }
  }
  distance = abs(acting->x - enemy->x) + abs(acting->y - enemy->y);
  if (UnitType_GetAttackKind(enemy->type) == ATTACK_KIND_DIRECT) {
    reach = slack + (Unit_GetUsableMovement(acting) +
                     Player_GetUnitMoveRange(MAP_UNIT_OWNER(STATE, enemy),
                                             enemy->type) +
                     1);
  } else {
    reach = slack +
            (Unit_GetUsableMovement(acting) +
             GetUnitBaseMaxRange(MAP_UNIT_OWNER(STATE, enemy), enemy->type));
  }
  if (distance > reach) {
    return FALSE;
  }
  return TRUE;
}
