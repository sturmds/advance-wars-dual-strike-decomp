#include "types.h"
#include "game/map_object.h"
#include "game/rng.h"
#include "game/Unk_02080440.h"
#include "game/object_attack_task.h"
#include "game/task.h"
#include "game/unit_action.h"

extern UnitAction data_0229d8c8;
extern const TaskScriptCmd data_02168380[];

void Combat_CalcDamageToTarget(int unitIndex, int target, s16* pos);
void ScreenConfig_RunActionHook(void);

#define DAMAGE (MAP_STATE->combatants[0].damageNoLuck)

#define TILE_HP_MASK 0x7f
#define TILE_HP_FLAG 0x80 /* kept as it is; meaning unknown */

/* The acting unit, from its destination, attacks the tile (x, y) that has
 * hp (likely a pipe seam): the RNG state is saved in data_0229d8c8, the
 * damage without luck comes off the tile's hp (not below 0), and the attack
 * task starts with flag. */
void ActingUnit_AttackTile(int x, int y, int flag) {
  int hp;
  ObjectAttackTask* task;

  data_0229d8c8.rngState = gRngState;
  Combat_CalcDamageToTarget(MAP_STATE->actingUnitIndex, MAP_OBJECT_TILE,
                            &MAP_STATE->actingUnitDestX);
  if ((MAP_STATE->tileHp[MAP_TILE_INDEX(x, y)] & TILE_HP_MASK) < DAMAGE) {
    hp = 0;
  } else {
    hp = (MAP_STATE->tileHp[MAP_TILE_INDEX(x, y)] & TILE_HP_MASK) - DAMAGE;
  }
  MAP_STATE->tileHp[MAP_TILE_INDEX(x, y)] =
      hp | (MAP_STATE->tileHp[MAP_TILE_INDEX(x, y)] & TILE_HP_FLAG);
  ScreenConfig_RunActionHook();
  task = (ObjectAttackTask*)Task_Create(data_02168380, TASK_LIST(3));
  task->tileX = x;
  task->tileY = y;
  task->attacker = MAP_STATE->actingUnit;
  task->object = NULL;
  task->flag = flag;
}
