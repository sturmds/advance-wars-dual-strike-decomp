#include "types.h"
#include "game/Unk_02080440.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;
extern u32 gLinkPlay;

void func_020b9d48(void);
void func_0201e954(void);
void AiShowAction_Dive(void);
void ActingUnit_FinishAction(void);
void UnitAction_Send(int action, int unitIndex, u8 target, u8 arg);

#define GAME (data_027c027c->unk_48[data_027c0284])

/* Unit command menu: Dive. Sends unit action 11 when gLinkPlay is set. */
void UnitMenu_Dive(void) {
  func_020b9d48();
  func_0201e954();
  AiShowAction_Dive();
  ActingUnit_FinishAction();
  if (gLinkPlay != 0) {
    UnitAction_Send(11, GAME->actingUnitIndex, 0, 0);
  }
}
