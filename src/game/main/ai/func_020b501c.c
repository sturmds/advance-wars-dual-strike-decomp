#include "types.h"
#include "game/ai_unit_action.h"
#include "game/ai/ai_unload.h"
#include "game/ai.h"

BOOL func_020b5098(void);
int func_020b507c(void);

void func_020b501c(void) {
  gAiUnloadTest = func_020b5098;
  gAiUnloadScore = func_020b507c;
  AiSeaTransport_ScoreUnloadTiles();
  if (gAiTurnState->stepFlags & AI_STEP_UNLOAD_SCORED) {
    func_020b5bf4();
  }
  func_020b5464();
}
