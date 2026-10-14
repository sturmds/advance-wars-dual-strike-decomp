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

#define CELL_SPRITES (MENU->battle.cellSprites)
#define SPRITE (CELL_SPRITES[side][slot])
#define OTHER_EFFECT           \
  (MENU->battle.unk_27c[other] \
       ->effects[Ov002_gBattleScene->sides[other].usesSecondary])

/* Starts a sprite in slot `slot` of the side's cell sprites, from the sprite of `cell`. With
 * flag 0 it also moves the step and target by the point of the slot. */
void Battle_StartCellSpriteOnSlot(int side, int cell, int slot, int flag,
                                  int param) {
  BattleSpriteTask* cellTask =
      (BattleSpriteTask*)MENU->battle.unk_310[side][cell].unk_00;
  int other = (side + 1) & 1;
  int taskSide;
  int taskOther;
  const BattleCellLayout* points = &MENU->battle.unk_27c[side]->layout->cell;

  SPRITE.pos.x = cellTask->unk_38;
  SPRITE.pos.y = cellTask->unk_3a;
  SPRITE.step.x = OTHER_EFFECT->unk_26.x;
  SPRITE.step.y = cellTask->unk_46 + OTHER_EFFECT->unk_26.y;
  SPRITE.target.x = cellTask->unk_38;
  SPRITE.target.y = cellTask->unk_3a + cellTask->unk_46;
  if (flag == 0) {
    SPRITE.step.x += points->unitOffsets[slot].x;
    SPRITE.step.y += points->unitOffsets[slot].y;
    SPRITE.target.x += points->unitOffsets[slot].x;
    SPRITE.target.y += points->unitOffsets[slot].y;
  }
  SPRITE.cell = (u8)cell;
  SPRITE.unk_14_3 = (u8)flag;
  SPRITE.unk_14_hi = (u8)param;
  taskSide = cellTask->side;
  taskOther = (taskSide + 1) & 1;
  SPRITE.task = func_ov002_02356604(
      MENU->battle.unk_48[taskOther], 1, CELL_SPRITES[taskSide][slot].pos.x,
      CELL_SPRITES[taskSide][slot].pos.y, cellTask->unk_3c,
      CELL_SPRITES[taskSide][slot].step.x, CELL_SPRITES[taskSide][slot].step.y,
      -1, cellTask->scale, cellTask->scale, (&MENU->colours[8])[taskOther], 8,
      1, taskSide);
  SPRITE.direction.x = OTHER_EFFECT->direction.x;
  SPRITE.direction.y = OTHER_EFFECT->direction.y;
}
