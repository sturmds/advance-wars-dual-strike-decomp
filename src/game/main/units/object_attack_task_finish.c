#include "types.h"
#include "game/Unk_02080440.h"
#include "game/object_attack_task.h"

extern u32 gLinkPlay;

void ActingUnit_SaveStartPos(void);
void ActingUnit_FinishAction(void);
void ActingUnitStart_Restore(void);
void UnitAction_Send(int action, int unitIndex, u8 target, u8 arg);

/* End of the attack task of ActingUnit_AttackTile: ends the acting unit's action, then, when the task's flag is set and
 * gLinkPlay is set, sends unit action 5 with the attacked tile. */
void ObjectAttackTask_Finish(ObjectAttackTask* task) {
  ActingUnit_SaveStartPos();
  ActingUnit_FinishAction();
  ActingUnitStart_Restore();
  if ((s16)task->flag == 0) {
    return;
  }
  if (gLinkPlay == 0) {
    return;
  }
  UnitAction_Send(5, MAP_STATE->actingUnitIndex, task->tileX, task->tileY);
}
