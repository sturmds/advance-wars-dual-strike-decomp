#include "types.h"
#include "game/Unk_02080440.h"

extern u32 gLinkPlay;

void func_020b9d48(void);
void func_0201e954(void);
void ActingUnit_SaveStartPos(void);
void UnitAction_Board(void);
void ActingUnit_FinishAction(void);
void ActingUnitStart_Restore(void);
void UnitAction_Send(int action, int unitIndex, u8 target, u8 arg);

/* Unit command menu: Board. Sends unit action 7
 * when gLinkPlay is set. */
void UnitMenu_Board(void) {
  func_020b9d48();
  func_0201e954();
  ActingUnit_SaveStartPos();
  UnitAction_Board();
  ActingUnit_FinishAction();
  ActingUnitStart_Restore();
  if (gLinkPlay != 0) {
    UnitAction_Send(7, MAP_STATE->actingUnitIndex, 0, 0);
  }
}
