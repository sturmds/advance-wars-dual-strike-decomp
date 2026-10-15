#include "types.h"
#include "game/Unk_02080440.h"
#include "game/ai/ai_show_action.h"

extern u32 gLinkPlay;

void func_020b9d48(void);
void func_0201e954(void);
void ActingUnit_FinishAction(void);
void UnitAction_Send(int action, int unitIndex, u8 target, u8 arg);

/* Unit command menu: Emerge. Sends unit action 12 when gLinkPlay is set. */
void UnitMenu_Emerge(void) {
  func_020b9d48();
  func_0201e954();
  AiShowAction_Emerge();
  ActingUnit_FinishAction();
  if (gLinkPlay != 0) {
    UnitAction_Send(12, MAP_STATE->actingUnitIndex, 0, 0);
  }
}
