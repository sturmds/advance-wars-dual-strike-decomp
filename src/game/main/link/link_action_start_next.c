#include "types.h"
#include "game/unit_action.h"

extern u16 data_0229d8c4;

void func_020800c0(void);
int LinkActionQueue_Pop(UnitAction* action);
int UnitAction_IsValid(UnitAction* action);
void AiTurn_SetState(int state);
void func_0208c1f4(int a0);

/* State 0 of LinkAction_Update. Takes the next queued action into gUnitAction; when it is a valid
 * one, calls AiTurn_SetState(3) and func_0208c1f4(0) and moves to state 4, which shows it. */
void LinkAction_StartNext(void) {
  func_020800c0();
  if (LinkActionQueue_Pop(&gUnitAction) != 1) {
    return;
  }
  if (UnitAction_IsValid(&gUnitAction) != 1) {
    return;
  }
  AiTurn_SetState(3);
  func_0208c1f4(0);
  data_0229d8c4 = 4;
}
