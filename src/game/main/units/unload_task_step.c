#include "types.h"
#include "game/Unk_02080440.h"
#include "game/task.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;
extern u32 gLinkPlay;
extern u8 data_022a6b34[];

void ActingUnit_UnloadCargo(int cargoSlot);
void ActingUnit_FinishAction(void);
void MenuDepth_Pop(void);
void func_020b9d48(void);
void func_020c8888(void);
s8 func_020bf260(void);
void UnitMenu_Open(void);
void UnitAction_SendUnload(int unitIndex, int a1, int a2);

#define STATE (data_027c027c->unk_48[data_027c0284])

/* Task step of the unload command (the task func_020ce158 creates with the
 * cargo slot in unk_68). When unk_6a is 0 it drops that cargo unit, and
 * while func_020bf260 returns 0, UnitMenu_Open runs (guess: the unit menu opens again)
 * with the view target on the acting unit and unk_1c set to 5. Otherwise the
 * action ends and, when gLinkPlay is set, UnitAction_SendUnload sends it. */
void UnloadTask_Step(Task* task) {
  if (task->unk_6a == 0) {
    ActingUnit_UnloadCargo(task->unk_68);
  }
  func_020b9d48();
  MenuDepth_Pop();
  if (task->unk_6a == 0) {
    func_020c8888();
    if (func_020bf260() == 0) {
      STATE->cursorX = STATE->actingUnitDestX;
      STATE->cursorY = STATE->actingUnitDestY;
      STATE->unk_1c = 5;
      UnitMenu_Open();
      return;
    }
  }
  STATE->unk_1c = 0;
  ActingUnit_FinishAction();
  if (gLinkPlay != 0) {
    UnitAction_SendUnload(STATE->actingUnitIndex, data_022a6b34[0],
                          data_022a6b34[1]);
  }
}
