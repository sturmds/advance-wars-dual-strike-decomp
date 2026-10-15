#include "types.h"
#include "game/BattleCellTask.h"
#include "game/BattleSpriteTask.h"
#include "game/battle_scene.h"
#include "game/menu_state.h"
#include "game/battle_fighter_sprites.h"

/* Case 4 of BattleCellTask_RunStep: starts the cell flashing (BattleCellTask_Flash),
 * 12 times when the other side's BattleSideData.unk_49 for its weapon is 2, else once. */
void BattleCellTask_StartFlash(BattleCellTask* task) {
  int other;

  task->flashing = 1;
  other = (task->side + 1) & 1;
  if (BATTLE_SIDE_DATA(other)
          ->unk_49[Ov002_gBattleScene->sides[other].usesSecondary] == 2) {
    task->flashCount = 12;
  } else {
    task->flashCount = 1;
  }
  task->flashTimer = 0;
}
