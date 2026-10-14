#include "types.h"
#include "game/battle_scene.h"
#include "game/menu_state.h"
#include "game/BattleSpriteTask.h"
#include "game/task.h"

extern MenuState gMenuState;

#define MENU ((MenuState*)&gMenuState)
#define FX (MENU->battle.motions[side][cell])
#define EFFECT                \
  (MENU->battle.unk_27c[side] \
       ->effects[Ov002_gBattleScene->sides[side].usesSecondary])

void Task_Destroy(void* task);
/* 1 when the sprite has left the screen; a guess from its bounds checks. */
int Battle_IsMotionOutOfBounds(int side, int cell);

/* Moves the flying sprites of both sides once a frame: waits out the delay, adds the
 * acceleration of the side's weapon effect to the velocity and the velocity to the
 * offset, and destroys a sprite that has left the screen. */
void Battle_UpdateFx(void) {
  BattleSpriteTask* task;
  int side;
  int cell;

  for (side = 0; side < BATTLE_SIDE_COUNT; side++) {
    for (cell = 0; cell < BATTLE_CELL_COUNT; cell++) {
      task = (BattleSpriteTask*)FX.unk_00;
      if (task == NULL) {
        continue;
      }
      if (FX.delay != 0) {
        FX.delay--;
        continue;
      }
      FX.velocityX += EFFECT->acceleration.x;
      FX.velocityY += EFFECT->acceleration.y;
      FX.speedX += FX.velocityX >> 8;
      FX.speedY += FX.velocityY >> 8;
      task->unk_48 = FX.speedX;
      task->unk_4a = FX.speedY;
      if (Battle_IsMotionOutOfBounds(side, cell) == 1) {
        if (task != NULL) {
          Task_Destroy(task);
        }
        FX.unk_00 = 0;
      }
    }
  }
}
