#include "types.h"
#include "game/ai_unit_action.h"

void Ai_BuildThreatMaps(void);
void AiUnit_MoveToOtherMap(void);
void AiUnit_SeekSeaTransport(void);
void AiLoadedTransport_HeadForSite(void);
void AiTransport_UnloadForCapture(void);
void AiUnit_WaitInPlace(void);

/* Queue routine for APCs and T-Copters that carry a unit, set by
 * AiAction_QueueLoadedTransports. AiTransport_Unload runs
 * AiTransport_UnloadForCapture alone on the same units. */
void AiLoadedTransport_Act(void) {
  Ai_BuildThreatMaps();
  AiLoadedTransport_HeadForSite();
  AiUnit_SeekSeaTransport();
  gAiTurnState->stepFlags |= AI_STEP_ATTACK_DONE;
  AiTransport_UnloadForCapture();
  AiLoadedTransport_HeadForSite();
  AiUnit_MoveToOtherMap();
  AiUnit_WaitInPlace();
}
