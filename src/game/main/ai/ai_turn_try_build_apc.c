#include "types.h"
#include "game/ai_build_state.h"
#include "game/ai_profile.h"
#include "game/unit_action.h"
#include "game/unit_stats.h"
#include "game/Unk_02080440.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;

/* data_027c027c->unk_48[data_027c0284], written so that the compiler loads
 * the index first, as the original does. */
#define STATE                                                             \
  (*(Unk_02080440_48**)((u8*)(data_027c0284 * sizeof(Unk_02080440_48*)) + \
                            (u32)data_027c027c + (u32) &                  \
                        ((Unk_02080440*)0)->unk_48))

s32 HwDiv32(s32 numerator, s32 denominator);
int Ai_CountUnitsOfType(int type);
int Unit_GetValuePerHp(int playerIndex, int unitType);
int AiSite_PickNearestFootWithProperty(int arg);
void AiTurn_QueueBuildAtSite(int index);

/* End-actions step 8, last sub-step. Tries to start building an APC: needs
 * the funds for one and, when the profile's unk_08[10] is nonzero, a
 * state->unk_00 (likely the day) no greater than it. AiSite_PickNearestFootWithProperty(1) picks
 * the factory (-1: none). Builds when APCs and T-Copters together make up
 * less than unk_08[4] percent of data_021619c0->unk_08 (Ai_CountUnitsOfAiClass(1)).
 * The name is a guess from the unit type. Returns 1 when it queued an
 * action. */
BOOL AiTurn_TryBuildApc(void) {
  Unk_02080440_48* state;
  int index;
  int transports;

  state = STATE;
  if ((u32)(Unit_GetValuePerHp(state->currentPlayer, UNIT_APC) * 10) >
      state->players[state->currentPlayer].funds) {
    return 0;
  }
  if (gAiProfile->unk_08[10] != 0 && state->day > gAiProfile->unk_08[10]) {
    return 0;
  }
  index = AiSite_PickNearestFootWithProperty(1);
  if (index == -1) {
    return 0;
  }
  transports = Ai_CountUnitsOfType(UNIT_APC);
  transports += Ai_CountUnitsOfType(UNIT_T_COPTER);
  if (data_021619c0->unk_08 == 0) {
    return 0;
  }
  if (HwDiv32(transports * 100, data_021619c0->unk_08) <
      gAiProfile->unk_08[4]) {
    gUnitAction.target = UNIT_APC;
  } else {
    return 0;
  }
  AiTurn_QueueBuildAtSite(index);
  return 1;
}
