#include "types.h"

void Ai_BuildThreatMaps(void);
void AiTransport_UnloadForCapture(void);

/* Queue routine for APCs and T-Copters that carry a unit, set by
 * AiAction_QueueTransportUnloads. */
void AiTransport_Unload(void) {
  Ai_BuildThreatMaps();
  AiTransport_UnloadForCapture();
}
