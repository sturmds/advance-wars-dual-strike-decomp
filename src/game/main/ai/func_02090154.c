#include "types.h"
#include "game/ai_build_state.h"

int Ai_CountUnitsOfAiClass(int arg);
int AiTurn_TryBuildTCopter(void);
int AiTurn_TryBuildLander(void);
int AiTurn_TryBuildBlackBoatTransport(void);
int AiTurn_TryBuildBlackBoat(void);
int AiTurn_TryBuildApc(void);

/* End-actions step 8. Tries five sub-steps in order and returns 1 at the first
 * one that returns 1 (needs another frame). Their purposes are not known. */
BOOL func_02090154(void) {
  data_021619c0->unk_08 = Ai_CountUnitsOfAiClass(1);
  if (AiTurn_TryBuildTCopter() == 1) {
    return 1;
  }
  if (AiTurn_TryBuildLander() == 1) {
    return 1;
  }
  if (AiTurn_TryBuildBlackBoatTransport() == 1) {
    return 1;
  }
  if (AiTurn_TryBuildBlackBoat() == 1) {
    return 1;
  }
  return AiTurn_TryBuildApc() == 1;
}
