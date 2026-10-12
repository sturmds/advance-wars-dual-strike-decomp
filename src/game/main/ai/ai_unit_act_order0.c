#include "types.h"

void Ai_BuildThreatMaps(void);
void AiUnit_MoveToOtherMap(void);
void AiUnit_MoveToEnemyProperty(void);
void func_020aac28(void);
void AiUnit_MoveToEnemyTarget(void);
void AiUnit_SeekSeaTransport(void);
void func_020af5f8(void);
void AiUnit_RetreatTowardSite(void);
void AiUnit_PlanAttack(void);
void AiUnit_WaitInPlace(void);
void AiUnit_HeadForObjectTarget(void);

/* AiUnit_ActByOrder runs these AI steps for units with AI order 0, and
 * for order 2 when func_0209dd7c is FALSE. The last one, AiUnit_WaitInPlace,
 * makes the unit stay on its tile (AiUnit_PerformAction with action 2). */
void AiUnit_ActOrder0(void) {
  Ai_BuildThreatMaps();
  func_020af5f8();
  AiUnit_PlanAttack();
  AiUnit_RetreatTowardSite();
  AiUnit_MoveToEnemyTarget();
  AiUnit_SeekSeaTransport();
  AiUnit_HeadForObjectTarget();
  AiUnit_MoveToEnemyProperty();
  func_020aac28();
  AiUnit_MoveToOtherMap();
  AiUnit_WaitInPlace();
}
