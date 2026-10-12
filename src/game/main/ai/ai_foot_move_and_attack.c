#include "types.h"
#include "game/Unk_02080440.h"
#include "game/ai_unit_action.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;

#define GAME (data_027c027c->unk_48[data_027c0284])

void Ai_BuildThreatMaps(void);
int Unit_CapturePoints(MapUnit* unit);
void AiUnit_MoveToOtherMap(void);
void AiUnit_SeekSeaTransport(void);
void AiFoot_PickAttackTarget(void);
void AiFoot_JoinCapturingUnitLoose(void);
void AiFoot_JoinCapturingUnit(void);
void AiFoot_HeadForCaptureSite(void);
void AiFoot_PickCaptureTarget(void);
void AiFoot_AttackCapturingUnit(void);
void func_020af5f8(void);
void AiUnit_RetreatToRepair(void);
void AiUnit_PlanAttack(void);
void AiUnit_WaitInPlace(void);

/* Queue routine for infantry and mechs (AiAction_QueueFootUnits2 collects units
 * whose UnitStats.aiClass is AI_CLASS_FOOT). AiUnit_PlanAttack picks direct or indirect
 * combat; the other steps are not named yet. */
void AiFoot_MoveAndAttack(void) {
  Ai_BuildThreatMaps();
  if (Unit_CapturePoints(GAME->actingUnit) >= 20) {
    AiFoot_PickCaptureTarget();
  }
  if (GAME->players[GAME->currentPlayer].aiBehaviour != AI_BEHAVIOUR_DEFENSE) {
    AiFoot_AttackCapturingUnit();
    AiFoot_PickAttackTarget();
  }
  if (GAME->actingUnit->hp <= 60) {
    AiFoot_JoinCapturingUnit();
  }
  func_020af5f8();
  AiFoot_PickCaptureTarget();
  if (GAME->actingUnit->hp > 60) {
    AiFoot_JoinCapturingUnit();
  }
  AiUnit_PlanAttack();
  AiFoot_JoinCapturingUnitLoose();
  if (GAME->actingUnit->hp <= 60) {
    AiUnit_RetreatToRepair();
  }
  AiFoot_HeadForCaptureSite();
  AiUnit_SeekSeaTransport();
  gAiTurnState->stepFlags |= AI_STEP_ATTACK_DONE;
  AiFoot_HeadForCaptureSite();
  AiUnit_MoveToOtherMap();
  AiUnit_WaitInPlace();
}
