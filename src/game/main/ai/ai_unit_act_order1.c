#include "types.h"

void Ai_ClearThreatMaps(void);
void AiUnit_MoveToOtherMap(void);
void AiUnit_MoveToEnemyProperty(void);
void func_020aac28(void);
void AiUnit_MoveToEnemyTarget(void);
void AiUnit_SeekSeaTransport(void);
void AiUnit_SeekSupply(void);
void AiUnit_PlanAttack(void);
void AiUnit_WaitInPlace(void);
void AiUnit_HeadForObjectTarget(void);

/* AiUnit_ActByOrder runs these AI steps for units with AI order 1. The
 * last one, AiUnit_WaitInPlace, makes the unit stay on its tile
 * (AiUnit_PerformAction with action 2). */
void AiUnit_ActOrder1(void) {
  Ai_ClearThreatMaps();
  AiUnit_SeekSupply();
  AiUnit_PlanAttack();
  AiUnit_MoveToEnemyTarget();
  AiUnit_SeekSeaTransport();
  AiUnit_HeadForObjectTarget();
  AiUnit_MoveToEnemyProperty();
  func_020aac28();
  AiUnit_MoveToOtherMap();
  AiUnit_WaitInPlace();
}
