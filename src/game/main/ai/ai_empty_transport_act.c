#include "types.h"

void Ai_BuildThreatMaps(void);
void AiUnit_MoveToOtherMap(void);
void AiEmptyTransport_WaitNearBase(void);
void AiEmptyTransport_GoToFootUnit(void);
void func_020af5f8(void);
void AiUnit_WaitInPlace(void);

/* Queue routine for APCs and T-Copters that carry nothing, set by
 * AiAction_QueueEmptyTransports. */
void AiEmptyTransport_Act(void) {
  Ai_BuildThreatMaps();
  func_020af5f8();
  AiEmptyTransport_GoToFootUnit();
  AiEmptyTransport_WaitNearBase();
  AiUnit_MoveToOtherMap();
  AiUnit_WaitInPlace();
}
