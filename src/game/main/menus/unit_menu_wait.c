#include "types.h"
#include "game/Unk_02080440.h"

extern u32 gLinkPlay;
extern u8 data_022a6b34[];

void func_020b9d48(void);
void func_0201e954(void);
void ActingUnit_SaveStartPos(void);
void ActingUnit_FinishAction(void);
void ActingUnitStart_Restore(void);
void UnitAction_SendUnload(int unitIndex, int a1, int a2);
void UnitAction_Send(int action, int unitIndex, u8 target, u8 arg);

/* Unit command menu: Wait. When gLinkPlay is set, sends unit action 2, or
 * through UnitAction_SendUnload when either byte of data_022a6b34 is nonzero. */
void UnitMenu_Wait(void) {
  func_020b9d48();
  func_0201e954();
  ActingUnit_SaveStartPos();
  ActingUnit_FinishAction();
  ActingUnitStart_Restore();
  if (gLinkPlay != 0) {
    if (data_022a6b34[0] == 0 && data_022a6b34[1] == 0) {
      UnitAction_Send(2, MAP_STATE->actingUnitIndex, 0, 0);
    } else {
      UnitAction_SendUnload(MAP_STATE->actingUnitIndex, data_022a6b34[0],
                            data_022a6b34[1]);
    }
  }
}
