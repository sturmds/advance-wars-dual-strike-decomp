#include "types.h"

void Ai_BuildThreatMaps(void);
void AiUnit_MoveToOtherMap(void);
void AiUnit_MoveToEnemyProperty(void);
void func_020aa1ec(void);
void AiUnit_MoveToEnemyTarget(void);
void AiUnit_SeekSeaTransport(void);
void func_020af5f8(void);
void AiUnit_RetreatTowardSite(void);
void AiUnit_PlanAttack(void);
void AiUnit_WaitInPlace(void);
void AiUnit_HeadForObjectTarget(void);

/* AiUnit_ActByOrder runs these AI steps for units with AI order 5, and
 * for order 6 when Player_HasHq is FALSE for the current player. The
 * last one, AiUnit_WaitInPlace, makes the unit stay on its tile
 * (AiUnit_PerformAction with action 2). */
void AiUnit_ActOrder5(void) {
  Ai_BuildThreatMaps();
  func_020af5f8();
  AiUnit_PlanAttack();
  AiUnit_RetreatTowardSite();
  func_020aa1ec();
  AiUnit_HeadForObjectTarget();
  AiUnit_MoveToEnemyTarget();
  AiUnit_SeekSeaTransport();
  AiUnit_MoveToEnemyProperty();
  AiUnit_MoveToOtherMap();
  AiUnit_WaitInPlace();
}
