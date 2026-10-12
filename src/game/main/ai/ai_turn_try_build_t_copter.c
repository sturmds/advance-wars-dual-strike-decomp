#include "types.h"
#include "game/ai_build_state.h"
#include "game/ai_profile.h"
#include "game/ai_unit_action.h"
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
int AiTurn_TryBuildTCopterForFoot(void);
int AiSite_PickNearestFoot(int arg);
int AiSite_PickFirst(int arg);
void AiTurn_QueueBuildAtSite(int index);

/* End-actions step 8, first sub-step. Tries to start building a T-Copter:
 * needs the funds for one and, when gAiTurnState->propertyRegions is 1 and
 * the profile's unk_08[11] is nonzero, a state->unk_00 (likely the day) no
 * greater than it. AiTurn_TryBuildTCopterForFoot runs first and ends the step when it
 * returns 1. AiSite_PickNearestFoot(2), then AiSite_PickFirst(2), pick the factory
 * (-1: none). Builds when T-Copters make up less than unk_08[3] percent of
 * data_021619c0->unk_08. The name is a guess from the unit type. Returns 1
 * when it queued an action. */
BOOL AiTurn_TryBuildTCopter(void) {
  Unk_02080440_48* state;
  int index;
  int copters;

  state = STATE;
  if ((u32)(Unit_GetValuePerHp(state->currentPlayer, UNIT_T_COPTER) * 10) >
      state->players[state->currentPlayer].funds) {
    return 0;
  }
  if (gAiTurnState->propertyRegions == 1 && gAiProfile->unk_08[11] != 0 &&
      state->day > gAiProfile->unk_08[11]) {
    return 0;
  }
  if (AiTurn_TryBuildTCopterForFoot() == 1) {
    return 1;
  }
  index = AiSite_PickNearestFoot(2);
  if (index == -1) {
    index = AiSite_PickFirst(2);
    if (index == -1) {
      return 0;
    }
  }
  copters = Ai_CountUnitsOfType(UNIT_T_COPTER);
  if (data_021619c0->unk_08 == 0) {
    return 0;
  }
  if (HwDiv32(copters * 100, data_021619c0->unk_08) < gAiProfile->unk_08[3]) {
    gUnitAction.target = UNIT_T_COPTER;
  } else {
    return 0;
  }
  AiTurn_QueueBuildAtSite(index);
  return 1;
}
