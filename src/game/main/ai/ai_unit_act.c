#include "types.h"
#include "game/Unk_02080440.h"
#include "game/map_unit.h"
#include "game/unit_stats.h"
#include "game/ai_unit_action.h"
#include "game/ai/ai_unit_act.h"
#include "game/ai/ai_perform_action.h"
#include "game/ai/ai_threat_maps.h"
#include "game/ai/ai_threat.h"
#include "game/ai/ai_find_target.h"
#include "game/ai/ai_transport_act.h"
#include "game/ai/ai_foot.h"
#include "game/ai/ai_service.h"
#include "game/ai/ai_unit_move.h"
#include "game/ai/ai_sea_units.h"
#include "game/ai/ai_special_units.h"

/* The functions are in reverse address order: mwccarm emits a file's
 * functions last to first. */

#define MAP_STATE (data_027c027c->unk_48[data_027c0284])

BOOL func_0209dd7c(void);
BOOL Player_HasHq(int player);
int Unit_CapturePoints(MapUnit* unit);

/* AiUnit_ActByOrderFog, the queue routine of AiAction_QueueAllDirectUnitsFog,
 * runs this for units with any AI order but 1, 7 and 10. Same steps as
 * AiFogDirect_ActOrders1_10; AiUnit_PlanAttack picks the attack. */
void AiFogDirect_ActOtherOrders(void) {
  Ai_BuildThreatMaps();
  func_020af5f8();
  AiUnit_PlanAttack();
}

/* AiUnit_ActByOrderFog, the queue routine of AiAction_QueueAllDirectUnitsFog,
 * runs this for units with AI order 1 or 10. Same steps as
 * AiFogDirect_ActOtherOrders; AiUnit_PlanAttack picks the attack. */
void AiFogDirect_ActOrders1_10(void) {
  Ai_BuildThreatMaps();
  func_020af5f8();
  AiUnit_PlanAttack();
}

/* AiUnit_ActByOrderFog, the queue routine of AiAction_QueueAllDirectUnitsFog,
 * runs this for units with AI order 7. Ai_ClearThreatMaps clears the per-tile
 * AI maps (MapTiles.threatMax and threatSum). */
void AiFogDirect_ActOrder7(void) {
  Ai_ClearThreatMaps();
  AiUnit_AttackAdjacent();
}

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

/* AiUnit_ActByOrder runs these AI steps for units with AI order 2 when
 * func_0209dd7c is TRUE (otherwise the order 0 steps). The last one,
 * AiUnit_WaitInPlace, makes the unit stay on its tile (AiUnit_PerformAction
 * with action 2). */
void AiUnit_ActOrder2(void) {
  Ai_BuildThreatMaps();
  func_020af5f8();
  AiUnit_PlanAttack();
  AiUnit_RetreatTowardSite();
  func_020aac28();
  AiUnit_SeekSeaTransport();
  AiUnit_HeadForObjectTarget();
  AiUnit_MoveToEnemyProperty();
  AiUnit_MoveToEnemyTarget();
  AiUnit_MoveToOtherMap();
  AiUnit_WaitInPlace();
}

/* AiUnit_ActByOrder runs these AI steps for units with AI order 3. The
 * last one, AiUnit_WaitInPlace, makes the unit stay on its tile
 * (AiUnit_PerformAction with action 2). */
void AiUnit_ActOrder3(void) {
  Ai_BuildThreatMaps();
  func_020af5f8();
  AiUnit_PlanAttack();
  AiUnit_RetreatTowardSite();
  func_020aa9c4();
  AiUnit_SeekSeaTransport();
  AiUnit_HeadForObjectTarget();
  AiUnit_MoveToEnemyProperty();
  AiUnit_MoveToEnemyTarget();
  AiUnit_MoveToOtherMap();
  AiUnit_WaitInPlace();
}

/* AiUnit_ActByOrder runs these AI steps for units with AI order 4. The
 * last one, AiUnit_WaitInPlace, makes the unit stay on its tile
 * (AiUnit_PerformAction with action 2). */
void AiUnit_ActOrder4(void) {
  Ai_BuildThreatMaps();
  func_020af5f8();
  AiUnit_PlanAttack();
  AiUnit_RetreatTowardSite();
  func_020aa4a4();
  AiUnit_SeekSeaTransport();
  AiUnit_HeadForObjectTarget();
  AiUnit_MoveToEnemyTarget();
  AiUnit_MoveToEnemyProperty();
  AiUnit_MoveToOtherMap();
  AiUnit_WaitInPlace();
}

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

/* AiUnit_ActByOrder runs these AI steps for units with AI order 6 when
 * Player_HasHq is TRUE for the current player (otherwise the order 5
 * steps). The last one, AiUnit_WaitInPlace, makes the unit stay on its tile
 * (AiUnit_PerformAction with action 2). */
void AiUnit_ActOrder6(void) {
  Ai_BuildThreatMaps();
  func_020af5f8();
  AiUnit_PlanAttack();
  AiUnit_RetreatTowardSite();
  func_020aa068();
  AiUnit_SeekSeaTransport();
  AiUnit_HeadForObjectTarget();
  AiUnit_MoveToOtherMap();
  AiUnit_WaitInPlace();
}

/* AiUnit_ActByOrder runs this for units with AI order 7: the unit stays
 * on its tile (action 2) without running any AI step. */
void AiUnit_ActOrder7(void) {
  MapUnit* unit = data_027c027c->unk_48[data_027c0284]->actingUnit;

  AiUnit_PerformAction(unit->x, unit->y, 2, 0, 0);
}

/* AiUnit_ActByOrder runs these AI steps for units with AI order 8. The
 * last one, AiUnit_WaitInPlace, makes the unit stay on its tile
 * (AiUnit_PerformAction with action 2). */
void AiUnit_ActOrder8(void) {
  Ai_BuildThreatMaps();
  func_020af5f8();
  AiUnit_PlanAttack();
  AiUnit_RetreatTowardSite();
  func_020a9f64();
  AiUnit_SeekSeaTransport();
  AiUnit_HeadForObjectTarget();
  AiUnit_WaitInPlace();
}

/* AiUnit_ActByOrder runs these AI steps for units with AI order 9. The
 * last one, AiUnit_WaitInPlace, makes the unit stay on its tile
 * (AiUnit_PerformAction with action 2). */
void AiUnit_ActOrder9(void) {
  Ai_BuildThreatMaps();
  func_020af5f8();
  AiUnit_PlanAttack();
  AiUnit_RetreatTowardSite();
  func_020a9e44();
  AiUnit_SeekSeaTransport();
  AiUnit_HeadForObjectTarget();
  AiUnit_WaitInPlace();
}

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

/* AiUnit_ActByOrderGroup runs this for indirect units with any AI order
 * but 1, 7 and 10. AiUnit_PlanAttack picks the attack. */
void AiIndirect_ActOtherOrders(void) {
  Ai_BuildThreatMaps();
  func_020af5f8();
  AiUnit_PlanAttack();
}

/* AiUnit_ActByOrderGroup runs this for indirect units with AI order 1, 7
 * or 10. Ai_ClearThreatMaps clears the per-tile AI maps (MapTiles.threatMax and
 * threatSum); AiUnit_PlanAttack picks the attack. */
void AiIndirect_ActOrders1_7_10(void) {
  Ai_ClearThreatMaps();
  AiUnit_PlanAttack();
}

/* AiIndirect_MoveByOrder step for an indirect unit with AI order 0.
 * AiUnit_PlanAttack only runs in Fog of War or rain. */
void AiIndirect_MoveOrder0(void) {
  Ai_BuildThreatMaps();
  func_020af5f8();
  /* rain also brings fog */
  if (MAP_STATE->unk_1c4->fog != 0 ||
      MAP_STATE->unk_1c4->weather == WEATHER_RAIN) {
    AiUnit_PlanAttack();
  }
  AiUnit_RetreatTowardSite();
  AiUnit_MoveToEnemyTarget();
  AiUnit_SeekSeaTransport();
  AiUnit_HeadForObjectTarget();
  AiUnit_MoveToEnemyProperty();
  func_020aac28();
  AiUnit_MoveToOtherMap();
  AiUnit_WaitInPlace();
}

/* AiIndirect_MoveByOrder step for an indirect unit with AI order 1.
 * AiUnit_PlanAttack only runs in Fog of War or rain. */
void AiIndirect_MoveOrder1(void) {
  Ai_ClearThreatMaps();
  AiUnit_SeekSupply();
  /* rain also brings fog */
  if (MAP_STATE->unk_1c4->fog != 0 ||
      MAP_STATE->unk_1c4->weather == WEATHER_RAIN) {
    AiUnit_PlanAttack();
  }
  AiUnit_MoveToEnemyTarget();
  AiUnit_SeekSeaTransport();
  AiUnit_HeadForObjectTarget();
  AiUnit_MoveToEnemyProperty();
  func_020aac28();
  AiUnit_MoveToOtherMap();
  AiUnit_WaitInPlace();
}

/* AiIndirect_MoveByOrder step for an indirect unit with AI order 2, when
 * func_0209dd7c is true.
 * AiUnit_PlanAttack only runs in Fog of War or rain. */
void AiIndirect_MoveOrder2(void) {
  Ai_BuildThreatMaps();
  func_020af5f8();
  /* rain also brings fog */
  if (MAP_STATE->unk_1c4->fog != 0 ||
      MAP_STATE->unk_1c4->weather == WEATHER_RAIN) {
    AiUnit_PlanAttack();
  }
  AiUnit_RetreatTowardSite();
  func_020aac28();
  AiUnit_SeekSeaTransport();
  AiUnit_HeadForObjectTarget();
  AiUnit_MoveToEnemyProperty();
  AiUnit_MoveToEnemyTarget();
  AiUnit_MoveToOtherMap();
  AiUnit_WaitInPlace();
}

/* AiIndirect_MoveByOrder step for an indirect unit with AI order 3.
 * AiUnit_PlanAttack only runs in Fog of War or rain. */
void AiIndirect_MoveOrder3(void) {
  Ai_BuildThreatMaps();
  func_020af5f8();
  /* rain also brings fog */
  if (MAP_STATE->unk_1c4->fog != 0 ||
      MAP_STATE->unk_1c4->weather == WEATHER_RAIN) {
    AiUnit_PlanAttack();
  }
  AiUnit_RetreatTowardSite();
  func_020aa9c4();
  AiUnit_SeekSeaTransport();
  AiUnit_HeadForObjectTarget();
  AiUnit_MoveToEnemyProperty();
  AiUnit_MoveToEnemyTarget();
  AiUnit_MoveToOtherMap();
  AiUnit_WaitInPlace();
}

/* AiIndirect_MoveByOrder step for an indirect unit with AI order 4.
 * AiUnit_PlanAttack only runs in Fog of War or rain. */
void AiIndirect_MoveOrder4(void) {
  Ai_BuildThreatMaps();
  func_020af5f8();
  /* rain also brings fog */
  if (MAP_STATE->unk_1c4->fog != 0 ||
      MAP_STATE->unk_1c4->weather == WEATHER_RAIN) {
    AiUnit_PlanAttack();
  }
  AiUnit_RetreatTowardSite();
  func_020aa4a4();
  AiUnit_SeekSeaTransport();
  AiUnit_HeadForObjectTarget();
  AiUnit_MoveToEnemyTarget();
  AiUnit_MoveToEnemyProperty();
  AiUnit_MoveToOtherMap();
  AiUnit_WaitInPlace();
}

/* AiIndirect_MoveByOrder step for an indirect unit with AI order 5.
 * AiUnit_PlanAttack only runs in Fog of War or rain. */
void AiIndirect_MoveOrder5(void) {
  Ai_BuildThreatMaps();
  func_020af5f8();
  /* rain also brings fog */
  if (MAP_STATE->unk_1c4->fog != 0 ||
      MAP_STATE->unk_1c4->weather == WEATHER_RAIN) {
    AiUnit_PlanAttack();
  }
  AiUnit_RetreatTowardSite();
  func_020aa1ec();
  AiUnit_HeadForObjectTarget();
  AiUnit_MoveToEnemyTarget();
  AiUnit_SeekSeaTransport();
  AiUnit_MoveToEnemyProperty();
  AiUnit_MoveToOtherMap();
  AiUnit_WaitInPlace();
}

/* AiIndirect_MoveByOrder step for an indirect unit with AI order 6, when
 * Player_HasHq is true for the player. AiUnit_PlanAttack only runs in Fog
 * of War or rain. */
void AiIndirect_MoveOrder6(void) {
  Ai_BuildThreatMaps();
  func_020af5f8();
  /* rain also brings fog */
  if (MAP_STATE->unk_1c4->fog != 0 ||
      MAP_STATE->unk_1c4->weather == WEATHER_RAIN) {
    AiUnit_PlanAttack();
  }
  AiUnit_RetreatTowardSite();
  func_020aa068();
  AiUnit_SeekSeaTransport();
  AiUnit_HeadForObjectTarget();
  AiUnit_MoveToOtherMap();
  AiUnit_WaitInPlace();
}

/* AiIndirect_MoveByOrder step for an indirect unit with AI order 7.
 * AiUnit_PlanAttack only runs in Fog of War or rain, then the unit waits
 * where it is (action 2). */
void AiIndirect_MoveOrder7(void) {
  /* rain also brings fog */
  if (MAP_STATE->unk_1c4->fog != 0 ||
      MAP_STATE->unk_1c4->weather == WEATHER_RAIN) {
    AiUnit_PlanAttack();
  }
  AiUnit_PerformAction(MAP_STATE->actingUnit->x, MAP_STATE->actingUnit->y, 2, 0,
                       0);
}

/* AiIndirect_MoveByOrder step for an indirect unit with AI order 8.
 * AiUnit_PlanAttack only runs in Fog of War or rain. */
void AiIndirect_MoveOrder8(void) {
  Ai_BuildThreatMaps();
  func_020af5f8();
  /* rain also brings fog */
  if (MAP_STATE->unk_1c4->fog != 0 ||
      MAP_STATE->unk_1c4->weather == WEATHER_RAIN) {
    AiUnit_PlanAttack();
  }
  AiUnit_RetreatTowardSite();
  func_020a9f64();
  AiUnit_SeekSeaTransport();
  AiUnit_HeadForObjectTarget();
  AiUnit_WaitInPlace();
}

/* AiIndirect_MoveByOrder step for an indirect unit with AI order 9.
 * AiUnit_PlanAttack only runs in Fog of War or rain. */
void AiIndirect_MoveOrder9(void) {
  Ai_BuildThreatMaps();
  func_020af5f8();
  /* rain also brings fog */
  if (MAP_STATE->unk_1c4->fog != 0 ||
      MAP_STATE->unk_1c4->weather == WEATHER_RAIN) {
    AiUnit_PlanAttack();
  }
  AiUnit_RetreatTowardSite();
  func_020a9e44();
  AiUnit_SeekSeaTransport();
  AiUnit_HeadForObjectTarget();
  AiUnit_WaitInPlace();
}

/* AiIndirect_MoveByOrder step for an indirect unit with AI order 10.
 * AiUnit_PlanAttack only runs in Fog of War or rain. */
void AiIndirect_MoveOrder10(void) {
  Ai_ClearThreatMaps();
  AiUnit_SeekSupply();
  /* rain also brings fog */
  if (MAP_STATE->unk_1c4->fog != 0 ||
      MAP_STATE->unk_1c4->weather == WEATHER_RAIN) {
    AiUnit_PlanAttack();
  }
  func_020a9f64();
  AiUnit_SeekSeaTransport();
  AiUnit_HeadForObjectTarget();
  AiUnit_WaitInPlace();
}

/* Queue routine for infantry and mechs partway through a capture, set by
 * AiAction_QueueCapturingUnits. Starts like AiFoot_MoveAndCapture, then
 * runs AiFoot_KeepCapturing. */
void AiFoot_ContinueCapture(void) {
  Ai_BuildThreatMaps();
  if (Unit_CapturePoints(MAP_STATE->actingUnit) >= 20) {
    AiFoot_PickCaptureTarget();
  }
  if (MAP_STATE->players[MAP_STATE->currentPlayer].aiBehaviour !=
      AI_BEHAVIOUR_DEFENSE) {
    AiFoot_AttackCapturingUnit();
  }
  AiFoot_KeepCapturing();
}

/* Queue routine for infantry and mechs set by AiAction_QueueFootUnits. Runs the
 * same first steps as AiFoot_MoveAndAttack and stops before the attack
 * dispatcher. Unit_CapturePoints calls CaptureIncrease. */
void AiFoot_MoveAndCapture(void) {
  Ai_BuildThreatMaps();
  if (Unit_CapturePoints(MAP_STATE->actingUnit) >= 20) {
    AiFoot_PickCaptureTarget();
  }
  if (MAP_STATE->players[MAP_STATE->currentPlayer].aiBehaviour !=
      AI_BEHAVIOUR_DEFENSE) {
    AiFoot_AttackCapturingUnit();
    AiFoot_PickAttackTarget();
  }
  if (MAP_STATE->actingUnit->hp <= 60) {
    AiFoot_JoinCapturingUnit();
  }
  func_020af5f8();
  AiFoot_PickCaptureTarget();
  if (MAP_STATE->actingUnit->hp > 60) {
    AiFoot_JoinCapturingUnit();
  }
}

/* Queue routine for infantry and mechs (AiAction_QueueFootUnits2 collects units
 * whose UnitStats.aiClass is AI_CLASS_FOOT). AiUnit_PlanAttack picks direct or indirect
 * combat; the other steps are not named yet. */
void AiFoot_MoveAndAttack(void) {
  Ai_BuildThreatMaps();
  if (Unit_CapturePoints(MAP_STATE->actingUnit) >= 20) {
    AiFoot_PickCaptureTarget();
  }
  if (MAP_STATE->players[MAP_STATE->currentPlayer].aiBehaviour !=
      AI_BEHAVIOUR_DEFENSE) {
    AiFoot_AttackCapturingUnit();
    AiFoot_PickAttackTarget();
  }
  if (MAP_STATE->actingUnit->hp <= 60) {
    AiFoot_JoinCapturingUnit();
  }
  func_020af5f8();
  AiFoot_PickCaptureTarget();
  if (MAP_STATE->actingUnit->hp > 60) {
    AiFoot_JoinCapturingUnit();
  }
  AiUnit_PlanAttack();
  AiFoot_JoinCapturingUnitLoose();
  if (MAP_STATE->actingUnit->hp <= 60) {
    AiUnit_RetreatToRepair();
  }
  AiFoot_HeadForCaptureSite();
  AiUnit_SeekSeaTransport();
  gAiTurnState->stepFlags |= AI_STEP_ATTACK_DONE;
  AiFoot_HeadForCaptureSite();
  AiUnit_MoveToOtherMap();
  AiUnit_WaitInPlace();
}

/* Queue routine for the direct units AiAction_QueueAllDirectUnitsFog
 * collects: runs one of three action sequences on the acting unit, chosen
 * by its AI order. The flag test is the inverse of the one in
 * AiUnit_ActByOrderGroup. */
void AiUnit_ActByOrderFog(void) {
  if (data_027c027c->unk_48[data_027c0284]->actingUnit->serviceWanted == 2) {
    gAiTurnState->stepFlags |= AI_STEP_NO_ADJUST;
  }

  switch (data_027c027c->unk_48[data_027c0284]->actingUnit->aiOrder) {
    case 0:
    case 2:
    case 3:
    case 4:
    case 5:
    case 6:
    case 8:
    case 9:
      AiFogDirect_ActOtherOrders();
      break;
    case 1:
    case 10:
      AiFogDirect_ActOrders1_10();
      break;
    case 7:
      AiFogDirect_ActOrder7();
      break;
  }
}

/* Queue routine for the units AiAction_QueueDirectUnits, AiAction_QueueAllDirectUnits and
 * AiAction_QueueAllDirectUnitsFog collect: runs the action sequence for the active unit's
 * AI order. */
void AiUnit_ActByOrder(void) {
  Unk_02080440_48* game = data_027c027c->unk_48[data_027c0284];

  switch (game->actingUnit->aiOrder) {
    case 0:
      AiUnit_ActOrder0();
      break;
    case 1:
      AiUnit_ActOrder1();
      break;
    case 2:
      if (func_0209dd7c() == TRUE) {
        AiUnit_ActOrder2();
      } else {
        AiUnit_ActOrder0();
      }
      break;
    case 3:
      AiUnit_ActOrder3();
      break;
    case 4:
      AiUnit_ActOrder4();
      break;
    case 5:
      AiUnit_ActOrder5();
      break;
    case 6:
      if (Player_HasHq(game->currentPlayer) == TRUE) {
        AiUnit_ActOrder6();
      } else {
        AiUnit_ActOrder5();
      }
      break;
    case 8:
      AiUnit_ActOrder8();
      break;
    case 9:
      AiUnit_ActOrder9();
      break;
    case 10:
      AiUnit_ActOrder10();
      break;
    /* needed to match: case 7 comes last */
    case 7:
      AiUnit_ActOrder7();
      break;
  }
}

/* Queue routine for the units AiAction_QueueIndirectUnits collects: runs one of two
 * action sequences on the active unit, chosen by its AI order. */
void AiUnit_ActByOrderGroup(void) {
  if (data_027c027c->unk_48[data_027c0284]->actingUnit->serviceWanted != 2) {
    gAiTurnState->stepFlags |= AI_STEP_NO_ADJUST;
  }

  switch (data_027c027c->unk_48[data_027c0284]->actingUnit->aiOrder) {
    case 0:
    case 2:
    case 3:
    case 4:
    case 5:
    case 6:
    case 8:
    case 9:
      AiIndirect_ActOtherOrders();
      break;
    case 1:
    case 7:
    case 10:
      AiIndirect_ActOrders1_7_10();
      break;
  }
}

/* Queue routine for indirect units, set by AiAction_QueueIndirectMoves:
 * runs the move sequence for the acting unit's AI order. Same shape as
 * AiUnit_ActByOrder for direct units. */
void AiIndirect_MoveByOrder(void) {
  Unk_02080440_48* game = data_027c027c->unk_48[data_027c0284];

  switch (game->actingUnit->aiOrder) {
    case 0:
      AiIndirect_MoveOrder0();
      break;
    case 1:
      AiIndirect_MoveOrder1();
      break;
    case 2:
      if (func_0209dd7c() == TRUE) {
        AiIndirect_MoveOrder2();
      } else {
        AiIndirect_MoveOrder0();
      }
      break;
    case 3:
      AiIndirect_MoveOrder3();
      break;
    case 4:
      AiIndirect_MoveOrder4();
      break;
    case 5:
      AiIndirect_MoveOrder5();
      break;
    case 6:
      if (Player_HasHq(game->currentPlayer) == TRUE) {
        AiIndirect_MoveOrder6();
      } else {
        AiIndirect_MoveOrder5();
      }
      break;
    case 7:
      AiIndirect_MoveOrder7();
      break;
    case 8:
      AiIndirect_MoveOrder8();
      break;
    case 9:
      AiIndirect_MoveOrder9();
      break;
    case 10:
      AiIndirect_MoveOrder10();
      break;
  }
}

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

/* Queue routine for APCs and T-Copters that carry a unit, set by
 * AiAction_QueueTransportUnloads. */
void AiTransport_Unload(void) {
  Ai_BuildThreatMaps();
  AiTransport_UnloadForCapture();
}

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

/* Queue routine for Black Boats and Landers, set by
 * AiAction_QueueSeaTransports. An empty Lander takes func_020af5f8, the
 * step foot units also run; a loaded Lander or a Black Boat takes
 * AiUnit_SeekSupply. */
void AiSeaTransport_Act(void) {
  Ai_BuildThreatMaps();
  if (MAP_STATE->actingUnit->type == UNIT_LANDER &&
      MAP_STATE->actingUnit->cargo[0] == 0) {
    func_020af5f8();
  } else {
    AiUnit_SeekSupply();
  }
  AiSeaTransport_Move();
  AiUnit_MoveToOtherMap();
  AiUnit_WaitInPlace();
}

/* Queue routine set by AiAction_QueueClass3Units. No unit type has AI
 * class 3, so retail never runs it. */
void AiClass3_Act(void) {
  Ai_BuildThreatMaps();
  AiClass3_SupplyUnit();
}

/* Queue routine for Black Boats, set by AiAction_QueueBlackBoats. */
void AiBlackBoat_Act(void) {
  Ai_BuildThreatMaps();
  AiBlackBoat_RepairUnit();
}

/* Queue routine set by AiAction_QueueBlackBombExplosions. */
void AiBlackBomb_Explode(void) { AiBlackBomb_ChooseExplosionTile(); }

/* Queue routine for Black Bombs, set by AiAction_QueueBlackBombs and
 * AiAction_QueueBlackBombsForEagle. */
void AiBlackBomb_Act(void) {
  func_0209b2b0(0);
  AiBlackBomb_MoveTowardEnemy();
}

/* Queue routine for Ooziums, set by AiAction_QueueOoziums. */
void AiOozium_Act(void) {
  AiOozium_BuildThreatMaps();
  AiOozium_MoveToTarget();
}
