#include "types.h"
#include "game/ai_unit_action.h"
#include "game/ai/ai_unload.h"

void func_020b5604(void) {
  gAiUnloadTest = AiSeaTransport_FewFootUnitsNoTarget;
  gAiUnloadScore = AiSeaTransport_ScoreFootUnitsVsProperties;
  AiSeaTransport_ScoreUnloadTiles();
  if (gAiTurnState->stepFlags & AI_STEP_UNLOAD_SCORED) {
    func_020b5bf4();
  }
}
