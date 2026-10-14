#include "types.h"
#include "game/ai_unit_action.h"
#include "game/rng.h"
#include "game/unit_action.h"
#include "game/Unk_02080440.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;

extern UnitAction data_0229d8c8;
extern u32 gLinkPlay;

void UnitAction_Send(int action, int unitIndex, u8 target, u8 arg);
void longjmp(void* p, int n);

#define STATE (data_027c027c->unk_48[data_027c0284])

/* Records an AI action that no unit takes in gUnitAction so the turn
 * can show it. Actions 15-18 are CO powers (the AI passes 16 after
 * CanUseCoPower and 17 after CanUseSuperPower); those are also copied
 * to data_0229d8c8 through UnitAction_Send when gLinkPlay is set. */
void AiPlayer_PerformAction(int action) {
  gUnitAction.action = action;
  gUnitAction.rngState = gRngState;
  gUnitAction.pending = TRUE;
  switch (action) {
    case 15:
    case 16:
    case 17:
    case 18:
      if (gLinkPlay != 0) {
        data_0229d8c8.rngState = gRngState;
        UnitAction_Send(action, 0, STATE->currentPlayer, 0);
      }
      break;
  }
  longjmp(data_0229ea0c, 1);
}
