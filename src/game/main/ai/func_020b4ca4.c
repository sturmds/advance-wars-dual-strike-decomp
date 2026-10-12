#include "types.h"
#include "game/ai_unit_action.h"

BOOL AiSeaTransport_CargoReachesTarget(void);
int func_020b4d04(void);
void func_020b5464(void);
void func_020b5bf4(void);

void func_020b4ca4(void) {
  gAiUnloadTest = AiSeaTransport_CargoReachesTarget;
  gAiUnloadScore = func_020b4d04;
  AiSeaTransport_ScoreUnloadTiles();
  if (gAiTurnState->stepFlags & AI_STEP_UNLOAD_SCORED) {
    func_020b5bf4();
  }
  func_020b5464();
}
