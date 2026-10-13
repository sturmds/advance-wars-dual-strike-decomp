#include "types.h"
#include "game/rng.h"
#include "game/Unk_02080440.h"
#include "game/object_attack_task.h"
#include "game/task.h"
#include "game/unit_action.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;
extern UnitAction data_0229d8c8;
extern const TaskScriptCmd data_02168380[];

void ActingUnit_AttackTile(int x, int y, int flag);
void Combat_CalcDamageToTarget(int unitIndex, int target, s16* pos);
void ScreenConfig_RunActionHook(void);

#define STATE (data_027c027c->unk_48[data_027c0284])

/* The acting unit, from its destination, attacks the map object at (x, y),
 * or the tile there when no object stands on it. The RNG state is saved in
 * data_0229d8c8, the damage without luck comes off the object's hp (not
 * below 0), and the attack task starts with flag. */
void ActingUnit_AttackObject(int x, int y, int flag) {
  MapObject* object;
  ObjectAttackTask* task;

  object = Map_GetObjectAt(x, y);
  if (object == NULL) {
    ActingUnit_AttackTile(x, y, flag);
    return;
  }
  data_0229d8c8.rngState = gRngState;
  Combat_CalcDamageToTarget(STATE->actingUnitIndex, object->kind,
                            &STATE->actingUnitDestX);
  if (object->hp < STATE->combatants[0].damageNoLuck) {
    object->hp = 0;
  } else {
    object->hp = object->hp - STATE->combatants[0].damageNoLuck;
  }
  ScreenConfig_RunActionHook();
  task = (ObjectAttackTask*)Task_Create(data_02168380, TASK_LIST(3));
  task->attacker = STATE->actingUnit;
  task->object = object;
  task->flag = flag;
}
