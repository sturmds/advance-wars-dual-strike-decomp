#include "types.h"
#include "game/rng.h"
#include "game/Unk_02080440.h"
#include "game/unit_action.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;
extern UnitAction data_0229d8c8;
extern u32 gLinkPlay;

void func_0201e954(void);
void UnitAction_ActivateCoPower(void);
void UnitAction_Send(int action, int unitIndex, u8 target, u8 arg);

#define GAME (data_027c027c->unk_48[data_027c0284])

/* Pop-up menu handler (table entry at 0x02165c20): activates the CO power.
 * With gLinkPlay set it first sends unit action 16 with the current player. */
void PowerMenu_ActivateCo(void) {
  func_0201e954();
  data_0229d8c8.rngState = gRngState;
  if (gLinkPlay != 0) {
    UnitAction_Send(16, 0, GAME->currentPlayer, 0);
  }
  UnitAction_ActivateCoPower();
}
