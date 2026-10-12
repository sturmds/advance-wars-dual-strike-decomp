#include "types.h"

void Ai_ClearThreatMaps(void);
void func_020a9f64(void);
void AiUnit_SeekSeaTransport(void);
void AiUnit_SeekSupply(void);
void AiUnit_PlanAttack(void);
void AiUnit_WaitInPlace(void);
void AiUnit_HeadForObjectTarget(void);

/* AiUnit_ActByOrder runs these AI steps for units with AI order 10. The
 * last one, AiUnit_WaitInPlace, makes the unit stay on its tile
 * (AiUnit_PerformAction with action 2). */
void AiUnit_ActOrder10(void) {
  Ai_ClearThreatMaps();
  AiUnit_SeekSupply();
  AiUnit_PlanAttack();
  func_020a9f64();
  AiUnit_SeekSeaTransport();
  AiUnit_HeadForObjectTarget();
  AiUnit_WaitInPlace();
}
