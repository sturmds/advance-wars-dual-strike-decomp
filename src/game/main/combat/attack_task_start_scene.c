#include "types.h"
#include "game/Unk_02080440.h"
#include "game/Unk_02088d04.h"
#include "game/task.h"

extern const TaskScriptCmd data_02168318[];

BOOL BattleScene_IsUnitOnRight(MapUnit* unitA, MapUnit* unitB);
void BattleScene_FillFromCombat(MapUnit* unitA, MapUnit* unitB,
                                Combatant* combatantA, Combatant* combatantB);

/* Step of the attack script (data_02168400): on the battle scene's game state, fills the
 * battle scene from the acting unit and the defender (task->unk_4c), the unit that sits left
 * first, and starts the scene task (data_02168318). */
void AttackTask_StartScene(Task* task) {
  MapUnit* defender = task->unk_4c;

  if (data_027c027c != data_027c0288.unk_00) {
    return;
  }
  if (!BattleScene_IsUnitOnRight(MAP_STATE->actingUnit, defender)) {
    BattleScene_FillFromCombat(MAP_STATE->actingUnit, defender,
                               &MAP_STATE->combatants[0],
                               &MAP_STATE->combatants[1]);
  } else {
    BattleScene_FillFromCombat(defender, MAP_STATE->actingUnit,
                               &MAP_STATE->combatants[1],
                               &MAP_STATE->combatants[0]);
  }
  Task_CreateChild(data_02168318, task);
}
