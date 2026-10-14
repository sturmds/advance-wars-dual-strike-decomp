#include "types.h"
#include "game/battle_scene.h"
#include "game/menu_state.h"
#include "game/BattleSpriteTask.h"

extern MenuState gMenuState;

#define MENU ((MenuState*)&gMenuState)

BattleSpriteTask* func_ov002_02356604(int table, int index, int x, int y,
                                      int unk_40, int offsetX, int offsetY,
                                      int unk_4c, int scaleX, int scaleY,
                                      int unk_54, int unk_56, int unk_59,
                                      int side);

/* Needed to match: read in place at each use (a local pointer gets other registers). */
#define POINT (points->offsets[cell])
#define CELL_SPRITES (MENU->battle.cellSprites)
#define SPRITE (CELL_SPRITES[side][cell])
#define OTHER_EFFECT           \
  (MENU->battle.unk_27c[other] \
       ->effects[Ov002_gBattleScene->sides[other].usesSecondary])

/* Starts the sprite of `cell` from the side's first cell task: the start position is the
 * task's, step and target add the cell's point, and the step adds the other side's effect. */
void Battle_StartCellSprite(int side, int cell, int param) {
  BattleSpriteTask* cellTask =
      (BattleSpriteTask*)MENU->battle.unk_310[side][0].unk_00;
  int other = (side + 1) & 1;
  int taskSide;
  int taskOther;
  const BattleFighterLayout* points =
      &MENU->battle.unk_27c[side]->layout->fighter;

  SPRITE.pos.x = cellTask->unk_38;
  SPRITE.pos.y = cellTask->unk_3a;
  SPRITE.step.x = POINT.x + OTHER_EFFECT->unk_26.x;
  SPRITE.step.y = cellTask->unk_46 + (POINT.y + OTHER_EFFECT->unk_26.y);
  SPRITE.target.x = cellTask->unk_38 + POINT.x;
  SPRITE.target.y = cellTask->unk_3a + POINT.y + cellTask->unk_46;
  SPRITE.cell = 0;
  SPRITE.unk_14_3 = 0;
  SPRITE.unk_14_hi = (u8)param;
  taskSide = cellTask->side;
  taskOther = (taskSide + 1) & 1;
  SPRITE.task = func_ov002_02356604(
      MENU->battle.unk_48[taskOther], 1, CELL_SPRITES[taskSide][cell].pos.x,
      CELL_SPRITES[taskSide][cell].pos.y, cellTask->unk_3c,
      CELL_SPRITES[taskSide][cell].step.x, CELL_SPRITES[taskSide][cell].step.y,
      -1, cellTask->scale, cellTask->scale, (&MENU->colours[8])[taskOther], 8,
      1, taskSide);
  SPRITE.direction.x = OTHER_EFFECT->direction.x;
  SPRITE.direction.y = OTHER_EFFECT->direction.y;
}
