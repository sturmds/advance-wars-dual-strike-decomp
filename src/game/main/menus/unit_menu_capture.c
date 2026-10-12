#include "types.h"
#include "game/Unk_02080440.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;
extern u32 gLinkPlay;

void func_020b9d48(void);
void func_0201e954(void);
void ActingUnit_SaveStartPos(void);
void UnitAction_Capture(void);
void ActingUnit_FinishAction(void);
void ActingUnitStart_Restore(void);
void UnitAction_Send(int action, int unitIndex, u8 target, u8 arg);

#define GAME (data_027c027c->unk_48[data_027c0284])

/* Unit command menu: Capture. Sends unit action 3
 * when gLinkPlay is set. */
void UnitMenu_Capture(void) {
  func_020b9d48();
  func_0201e954();
  ActingUnit_SaveStartPos();
  UnitAction_Capture();
  ActingUnit_FinishAction();
  ActingUnitStart_Restore();
  if (gLinkPlay != 0) {
    UnitAction_Send(3, GAME->actingUnitIndex, 0, 0);
  }
}
