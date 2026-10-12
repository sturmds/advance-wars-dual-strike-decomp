#include "types.h"
#include "game/Unk_02080440.h"
#include "game/Unk_02088d04.h"
#include "game/ai_unit_action.h"
#include "game/map_header.h"
#include "game/player.h"
#include "game/unit_action.h"

extern Unk_02080440* data_027c027c;
extern Unk_027c0280* data_027c0280;
extern int data_027c0284;
extern Unk_027c0288 data_027c0288;

#define GAME (data_027c027c->unk_48[data_027c0284])

void Screen_SetCurrent(void* state);
void Screen_SetCurrentGfx(void* state);
void Screen_SetCurrentBoth(void* state);
int func_020b9664(void);
void AiShowAction_BlackBombExplode(void);
void func_020b8230(void);
void AiShowAction_Emerge(void);
void AiShowAction_Dive(void);
void AiShowAction_ShowCargoUnit(int arg);
void AiShowAction_Action22(void);
void AiShowAction_AttackObject(void);
void AiShowAction_AttackUnit(void);
void AiShowAction_StartTurn(void);
void AiShowAction_BuildUnitAlt(void);
void AiShowAction_BuildUnit(void);
void func_020c8888(void);
void Ai_FlagCurrentPlayerEnterState18(void);
void UnitAction_ActivateTagPower(void);
void UnitAction_ActivateSuperPower(void);
void UnitAction_ActivateCoPower(void);
void UnitAction_SwapCo(void);
void UnitAction_MoveToOtherMap(void);
void func_020dd89c(void);
void func_020dd9a0(void);
void UnitAction_Supply(void);
void UnitAction_Join(void);
void UnitAction_Repair(int arg);
void UnitAction_Board(void);
void UnitAction_Capture(void);
void ActingUnit_FinishAction(void);

/* Second display step: runs the display routine of the action in
 * gUnitAction. Unless the action is 1, 13 to 21 or 27, the acting unit
 * first gets its final tile and is marked as waited. Most actions end by
 * going back to the queue; 4, 5, 8 and 15 and a failed 22 set the next
 * state themselves. Action 27 sets the AI behaviour of a player in both
 * game states. */
void AiShowAction_Step2(void) {
  Unk_027c0280* saved0280;
  Unk_02080440* saved027c;

  if (!(gUnitAction.action == 1 || gUnitAction.action == 14 ||
        gUnitAction.action == 13 || gUnitAction.action == 15 ||
        gUnitAction.action == 16 || gUnitAction.action == 17 ||
        gUnitAction.action == 18 || gUnitAction.action == 19 ||
        gUnitAction.action == 20 || gUnitAction.action == 21 ||
        gUnitAction.action == 27)) {
    GAME->actingUnit->flags |= MAP_UNIT_WAITED;
    GAME->actingUnit->x = GAME->actingUnitDestX;
    GAME->actingUnit->y = GAME->actingUnitDestY;
    func_020c8888();
  }
  switch (gUnitAction.action) {
    case 1:
      AiShowAction_BuildUnit();
      break;
    case 14:
      AiShowAction_BuildUnitAlt();
      break;
    case 13:
      AiShowAction_StartTurn();
      break;
    case 15:
      UnitAction_SwapCo();
      if (func_020b9664() == 1) {
        gAiTurnState->state = AI_STATE_RUN_QUEUE;
      } else if (GAME->players[GAME->currentPlayer].tagPowerHalf ==
                 TAG_POWER_BACK) {
        gAiTurnState->state = AI_STATE_START;
      } else {
        gAiTurnState->state = 6;
      }
      gAiTurnState->actionStep = 0;
      return;
    case 16:
      UnitAction_ActivateCoPower();
      break;
    case 17:
      UnitAction_ActivateSuperPower();
      break;
    case 18:
      UnitAction_ActivateTagPower();
      break;
    case 2:
      ActingUnit_FinishAction();
      break;
    case 3:
      UnitAction_Capture();
      ActingUnit_FinishAction();
      break;
    case 4:
      AiShowAction_AttackUnit();
      return;
    case 5:
      AiShowAction_AttackObject();
      return;
    case 7:
      UnitAction_Board();
      ActingUnit_FinishAction();
      break;
    case 8:
      AiShowAction_ShowCargoUnit(1);
      AiShowAction_ShowCargoUnit(0);
      gAiTurnState->actionStep = 10;
      return;
    case 10:
      UnitAction_Join();
      ActingUnit_FinishAction();
      break;
    case 6:
      UnitAction_Supply();
      break;
    case 11:
      AiShowAction_Dive();
      ActingUnit_FinishAction();
      break;
    case 12:
      AiShowAction_Emerge();
      ActingUnit_FinishAction();
      break;
    case 20:
      func_020b8230();
      break;
    case 21:
      Ai_FlagCurrentPlayerEnterState18();
      break;
    case 22:
      if (GAME->gameMode == GAME_MODE_CAMPAIGN &&
          GAME->unk_1c4->mapId == MAP_CAMPAIGN_FIRST + 18) {
        func_020dd9a0();
        ActingUnit_FinishAction();
      } else {
        AiShowAction_Action22();
        return;
      }
      break;
    case 23:
      AiShowAction_BlackBombExplode();
      break;
    case 24:
      UnitAction_Repair(gUnitAction.target - 1);
      ActingUnit_FinishAction();
      break;
    case 25:
      func_020dd89c();
      ActingUnit_FinishAction();
      break;
    case 26:
      ActingUnit_FinishAction();
      UnitAction_MoveToOtherMap();
      break;
    case 27:
      saved0280 = data_027c0280;
      saved027c = data_027c027c;
      Screen_SetCurrentBoth(data_027c0288.unk_00);
      GAME->players[gUnitAction.target].aiBehaviour = gUnitAction.unk_07;
      Screen_SetCurrentBoth(data_027c0288.unk_04);
      GAME->players[gUnitAction.target].aiBehaviour = gUnitAction.unk_07;
      Screen_SetCurrent(saved027c);
      Screen_SetCurrentGfx(saved0280);
      break;
  }
  gAiTurnState->state = AI_STATE_RUN_QUEUE;
  gAiTurnState->actionStep = 0;
}
