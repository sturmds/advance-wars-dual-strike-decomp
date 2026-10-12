#include "types.h"

void ActingUnit_SaveStartPos(void);
void ActingUnit_FinishAction(void);
void ActingUnitStart_Restore(void);
void func_020b9d48(void);

void func_020eef58(void) {
  ActingUnit_SaveStartPos();
  ActingUnit_FinishAction();
  ActingUnitStart_Restore();
  func_020b9d48();
}
