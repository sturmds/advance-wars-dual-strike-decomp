#include "types.h"
#include "game/ai_unit_action.h"

BOOL AiSeaTransport_FewFootUnitsNoTarget(void);
int AiSeaTransport_ScoreFootUnitsVsProperties(void);
void func_020b5bf4(void);

void func_020b5604(void) {
  gAiUnloadTest = AiSeaTransport_FewFootUnitsNoTarget;
  gAiUnloadScore = AiSeaTransport_ScoreFootUnitsVsProperties;
  AiSeaTransport_ScoreUnloadTiles();
  if (gAiTurnState->stepFlags & AI_STEP_UNLOAD_SCORED) {
    func_020b5bf4();
  }
}
