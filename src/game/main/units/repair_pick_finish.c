#include "types.h"
#include "game/Unk_02080440.h"
#include "game/task.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;
extern u32 gLinkPlay;

void func_020b9d48(void);
void MenuDepth_Pop(void);
void ActingUnit_FinishAction(void);
void UnitAction_Send(int action, int unitIndex, u8 target, u8 arg);

#define STATE (data_027c027c->unk_48[data_027c0284])

/* Script step of the repair tile picker (data_02166678) after the tile is
 * chosen: closes the menu, ends the unit's action, and when gLinkPlay is
 * set sends unit action 24 (repair) with the picked direction + 1 (task->unk_66
 * holds the direction). */
void RepairPick_Finish(Task* task) {
  func_020b9d48();
  MenuDepth_Pop();
  STATE->unk_1c = 0;
  ActingUnit_FinishAction();
  if (gLinkPlay != 0) {
    UnitAction_Send(24, STATE->actingUnitIndex, task->unk_66 + 1, 0);
  }
}
