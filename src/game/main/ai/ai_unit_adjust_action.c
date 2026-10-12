#include "types.h"
#include "game/Unk_02080440.h"
#include "game/ai_unit_action.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;

void AiUnit_HandleUnitAtTarget(int x, int y);
void AiUnit_WaitToAttack(int x, int y, u8* action, u8* target);
void AiUnit_UpgradeWaitAction(int x, int y, u8* action, u8* target, u8* arg);

#define STATE (data_027c027c->unk_48[data_027c0284])

/* Run by AiUnit_PerformAction before it records an action; the callees
 * may change action, target and arg. With AI_STEP_SWAP_CARGO
 * set, first swaps the acting unit's two cargo slots (and, for action 8,
 * target and arg with them) and clears the bit. AI_STEP_NO_ADJUST skips the rest. */
void AiUnit_AdjustAction(int x, int y, u8* action, u8* target, u8* arg) {
  u8 swap;

  if (gAiTurnState->stepFlags & AI_STEP_SWAP_CARGO) {
    swap = STATE->actingUnit->cargo[0];
    STATE->actingUnit->cargo[0] = STATE->actingUnit->cargo[1];
    STATE->actingUnit->cargo[1] = swap;
    if (*action == 8) {
      swap = *target;
      *target = *arg;
      *arg = swap;
    }
    gAiTurnState->stepFlags &= ~AI_STEP_SWAP_CARGO;
  }
  if (gAiTurnState->stepFlags & AI_STEP_NO_ADJUST) {
    return;
  }
  switch (*action) {
    case 2:
      if (!(gAiTurnState->stepFlags & AI_STEP_UNIT_AT_TARGET)) {
        AiUnit_WaitToAttack(x, y, action, target);
      }
      break;
    case 10:
      AiUnit_HandleUnitAtTarget(x, y);
      break;
  }
  AiUnit_UpgradeWaitAction(x, y, action, target, arg);
}
