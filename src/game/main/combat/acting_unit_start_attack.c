#include "types.h"
#include "game/rng.h"
#include "game/Unk_02080440.h"
#include "game/Unk_02088d04.h"
#include "game/task.h"
#include "game/unit_action.h"
#include "game/visual_mode.h"
#include "game/math.h"

extern UnitAction data_0229d8c8;
extern const TaskScriptCmd data_02168328[];
extern const TaskScriptCmd data_02168400[];

void Combat_SimulateChecked(int attackerIndex, int defenderIndex, s16* pos);
void func_020b9d48(void);
int Unit_GetChargeForHpLost(int player, int hpLost, int unitType);
void ScreenConfig_RunActionHook(void);
void ActingUnit_FinishAction(void);
void Player_AddPowerCharge(int player, int slot,
                           int amount); /* slot 0: front CO, 1: back CO */

#define MAP_STATE (data_027c027c->unk_48[data_027c0284])

/* HP as shown on the map: 1-10, rounded up. */
#define SHOWN_HP(hp) ((hp) != 0 ? HwDiv32((hp) - 1, 10) + 1 : 0)

/* The acting unit attacks the unit in slot defenderSlot. With slot 0 (no
 * unit) it only uses up one ammo. Otherwise it works out the battle (the RNG
 * state is saved in data_0229d8c8 first), gives each player meter charge
 * for the shown HP both sides lost, weighted by UnitStats.chargePerHp
 * (their own loss plus half of the other's; the back CO gets half of
 * that), and starts the battle task, which gets flag and the defender. */
void ActingUnit_StartAttack(int defenderSlot, int flag) {
  MapUnit* defender = &MAP_STATE->units[defenderSlot];
  u8 attackerOwner = MAP_UNIT_OWNER(MAP_STATE, MAP_STATE->actingUnit);
  u8 defenderOwner = MAP_UNIT_OWNER(MAP_STATE, defender);
  int attackerLoss;
  int defenderLoss;
  int attackerCharge;
  int defenderCharge;
  int mode;
  Task* task;

  if (defenderSlot == 0) {
    MAP_STATE->actingUnit->ammo--;
    func_020b9d48();
    ActingUnit_FinishAction();
    return;
  }

  data_0229d8c8.rngState = gRngState;
  Combat_SimulateChecked(MAP_STATE->actingUnitIndex, defenderSlot,
                         &MAP_STATE->actingUnitDestX);

  attackerLoss =
      Unit_GetChargeForHpLost(attackerOwner,
                              SHOWN_HP(MAP_STATE->actingUnit->hp) -
                                  SHOWN_HP(MAP_STATE->combatants[0].hp),
                              MAP_STATE->actingUnit->type);
  defenderLoss = Unit_GetChargeForHpLost(
      defenderOwner,
      SHOWN_HP(defender->hp) - SHOWN_HP(MAP_STATE->combatants[1].hp),
      defender->type);

  attackerCharge = attackerLoss + (defenderLoss >> 1);
  Player_AddPowerCharge(attackerOwner, 0, attackerCharge);
  defenderCharge = defenderLoss + (attackerLoss >> 1);
  Player_AddPowerCharge(defenderOwner, 0, defenderCharge);
  if (MAP_STATE->players[attackerOwner].back.id != 0) {
    Player_AddPowerCharge(attackerOwner, 1, attackerCharge >> 1);
  }
  if (MAP_STATE->players[defenderOwner].back.id != 0) {
    Player_AddPowerCharge(defenderOwner, 1, defenderCharge >> 1);
  }

  /* VISUAL_NONE starts the data_02168328 task, any other mode data_02168400.
   * VISUAL_D becomes VISUAL_C for a human current player and VISUAL_NONE
   * otherwise; the state at data_027c0288.unk_04 always uses VISUAL_NONE. */
  mode = MAP_STATE->visualMode;
  if (mode == VISUAL_D) {
    mode = MAP_STATE->players[MAP_STATE->currentPlayer].controller ==
                   PLAYER_CONTROLLER_HUMAN
               ? VISUAL_C
               : VISUAL_NONE;
  }
  if (data_027c027c == data_027c0288.unk_04) {
    mode = VISUAL_NONE;
  }
  /* needed to match: the task start is repeated in each case */
  switch (mode) {
    case VISUAL_A:
    case VISUAL_B:
    case VISUAL_C:
    default:
      ScreenConfig_RunActionHook();
      task = Task_Create(data_02168400, TASK_LIST(3));
      task->unk_64 = flag;
      task->unk_4c = defender;
      break;
    case VISUAL_NONE:
      ScreenConfig_RunActionHook();
      task = Task_Create(data_02168328, TASK_LIST(3));
      task->unk_64 = flag;
      task->unk_4c = defender;
      break;
  }
}
