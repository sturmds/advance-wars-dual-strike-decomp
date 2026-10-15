#include "types.h"
#include "game/ai_unit_action.h"
#include "game/ai/ai_unload.h"
#include "game/ai.h"

int func_020b4e84(void);

void func_020b4e24(void) {
  gAiUnloadTest = AiSeaTransport_CargoReachesHq;
  gAiUnloadScore = func_020b4e84;
  AiSeaTransport_ScoreUnloadTiles();
  if (gAiTurnState->stepFlags & AI_STEP_UNLOAD_SCORED) {
    func_020b5bf4();
  }
  func_020b5464();
}
