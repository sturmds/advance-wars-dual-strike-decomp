#include "types.h"
#include "game/BattleCellTask.h"
#include "game/BattleSideData.h"
#include "game/BattleSpriteTask.h"
#include "game/battle_scene.h"
#include "game/task.h"

#define MENU ((MenuState*)&gMenuState)
#define SPRITE (MENU->battle.cellSprites[side][i])
#define TARGET_CELL_TASK \
  ((BattleCellTask*)MENU->battle.unk_310[side][SPRITE.cell].unk_00)

void BattleCell_ApplyStep(int side, int step);
BOOL BattleCellSprite_Arrived(int side, int cell);
int BattleCellTask_GetCellOffsetX(BattleCellTask* task, int shot);
int BattleCellTask_GetCellOffsetY(BattleCellTask* task, int shot);

/* Moves the sprites flying at each side's HP cells: the other side's effect data
 * (unk_2e) is added to their direction. A sprite that arrives is destroyed; a hit sprite is
 * made at the cell, the cell's next step is set to 4, and the side's HP drain runs step
 * unk_14_hi. Needed to match: one variable holds the flying sprite and then the cell task, and the
 * angle is added as `- -angle`. Hit sprites take colours 10 and 11, one per side. */
void BattleCellSprites_Move(void) {
  int side;
  int i;
  BattleCellTask* task;

  for (side = 0; side < BATTLE_SIDE_COUNT; side++) {
    for (i = 0; i < BATTLE_CELL_COUNT; i++) {
      task = (BattleCellTask*)SPRITE.task;
      if (task != NULL) {
        SPRITE.direction.x +=
            MENU->battle.unk_27c[(side + 1) & 1]
                ->effects[Ov002_gBattleScene->sides[(side + 1) & 1]
                              .usesSecondary]
                ->unk_2e.x;
        SPRITE.direction.y +=
            MENU->battle.unk_27c[(side + 1) & 1]
                ->effects[Ov002_gBattleScene->sides[(side + 1) & 1]
                              .usesSecondary]
                ->unk_2e.y;
        SPRITE.step.x += SPRITE.direction.x >> 8;
        SPRITE.step.y += SPRITE.direction.y >> 8;
        ((BattleSpriteTask*)task)->unk_48 = SPRITE.step.x;
        ((BattleSpriteTask*)task)->unk_4a = SPRITE.step.y;
        if (BattleCellSprite_Arrived(side, i) == 1) {
          if (task != NULL) {
            Task_Destroy((Task*)task);
          }
          SPRITE.task = NULL;
          if (SPRITE.unk_14_3) {
            task = TARGET_CELL_TASK;
            BattleSpriteTask_CreateIfSet(
                MENU->battle.hitSpriteTables[task->side], 0, task->posX,
                task->posY, task->posZ, BattleCellTask_GetCellOffsetY(task, -1),
                BattleCellTask_GetCellOffsetX(task, -1) - -task->angle, -1,
                task->scale, task->scale, (&MENU->colours[10])[task->side], 8,
                1, task->side, task->hasPos2, task->pos2X, task->pos2Y,
                task->pos2Z, 8);
          } else {
            task = TARGET_CELL_TASK;
            BattleSpriteTask_CreateIfSet(
                MENU->battle.hitSpriteTables[task->side], 0, task->posX,
                task->posY, task->posZ, BattleCellTask_GetCellOffsetY(task, i),
                BattleCellTask_GetCellOffsetX(task, i) - -task->angle, -1,
                task->scale, task->scale, (&MENU->colours[10])[task->side], 8,
                1, task->side, task->hasPos2, task->pos2X, task->pos2Y,
                task->pos2Z, 8);
          }
          task = TARGET_CELL_TASK;
          task->steps[task->stepIndex] = 4;
          task->stepIndex++;
          if (task->stepIndex == 8) {
            task->stepIndex = 0;
          }
          BattleCell_ApplyStep(side, SPRITE.unk_14_hi);
        }
      }
    }
  }
}
