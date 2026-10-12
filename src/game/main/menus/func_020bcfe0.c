#include "types.h"
#include "game/Unk_02080440.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;
extern u32 gLinkPlay;

void func_020b9d48(void);
void func_0201e954(void);
void func_020dd89c(void);
void ActingUnit_FinishAction(void);
void UnitAction_Send(int action, int unitIndex, u8 target, u8 arg);

#define GAME (data_027c027c->unk_48[data_027c0284])

/* Handler of an item in the unit command menu (table entry at 0x02166040).
 * Runs unit action 25 as AiShowAction does, then sends it when gLinkPlay is set. */
void func_020bcfe0(void) {
  func_020b9d48();
  func_0201e954();
  func_020dd89c();
  ActingUnit_FinishAction();
  if (gLinkPlay != 0) {
    UnitAction_Send(25, GAME->actingUnitIndex, 0, 0);
  }
}
