#include "types.h"
#include "game/ai_unit_action.h"
#include "game/ai/ai_unload.h"
#include "game/ai.h"

int func_020b5198(void);

void func_020b5138(void) {
  gAiUnloadTest = AiSeaTransport_PropertiesOutweighArmed;
  gAiUnloadScore = func_020b5198;
  AiSeaTransport_ScoreUnloadTiles();
  if (gAiTurnState->stepFlags & AI_STEP_UNLOAD_SCORED) {
    func_020b5bf4();
  }
  func_020b5464();
}
