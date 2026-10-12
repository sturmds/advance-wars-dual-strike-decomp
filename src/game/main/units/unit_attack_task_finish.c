#include "types.h"
#include "game/Unk_02080440.h"
#include "game/task.h"
#include "game/unit_target_entry.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;
extern u32 gLinkPlay;
extern int data_022a6dd4;

void ActingUnit_SaveStartPos(void);
void ActingUnit_FinishAction(void);
void ActingUnitStart_Restore(void);
void UnitAction_Send(int action, int unitIndex, u8 target, u8 arg);
UnitTargetEntry* func_020dc524(int index);

#define STATE (data_027c027c->unk_48[data_027c0284])

/* End of the unit attack's battle task (started by ActingUnit_StartAttack): ends the acting unit's action, then, when
 * the task's flag is set and gLinkPlay is set, sends unit action 4 with the unit slot of the chosen target. */
void UnitAttackTask_Finish(Task* task) {
  UnitTargetEntry* target;

  ActingUnit_SaveStartPos();
  ActingUnit_FinishAction();
  ActingUnitStart_Restore();
  if (task->unk_64 == 0) {
    return;
  }
  target = func_020dc524(data_022a6dd4);
  if (gLinkPlay == 0) {
    return;
  }
  UnitAction_Send(4, STATE->actingUnitIndex, target->unitSlot, 0);
}
