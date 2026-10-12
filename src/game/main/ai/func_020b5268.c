#include "types.h"
#include "game/ai_unit_action.h"

BOOL AiSeaTransport_CargoReachesEnemyHq(void);
int func_020b52cc(void);
void func_020b5bf4(void);

void func_020b5268(void) {
  gAiUnloadTest = AiSeaTransport_CargoReachesEnemyHq;
  gAiUnloadScore = func_020b52cc;
  AiSeaTransport_ScoreUnloadTiles();
  if (gAiTurnState->stepFlags & AI_STEP_UNLOAD_SCORED) {
    func_020b5bf4();
  }
}
