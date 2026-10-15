#include "types.h"
#include "game/BattleCellTask.h"
#include "game/BattleSideData.h"
#include "game/BattleSpriteTask.h"
#include "game/battle_scene.h"
#include "game/menu_state.h"

#define MENU ((MenuState*)&gMenuState)
/* Needed to match: the effect is looked up again for every field, not kept in a local. */
#define SIDE_EFFECT(side)     \
  (MENU->battle.unk_27c[side] \
       ->effects[Ov002_gBattleScene->sides[side].usesSecondary])

BattleSpriteTask* func_ov002_02356604(const u32* table, int index, int x, int y,
                                      int z, int offsetX, int offsetY,
                                      int unk_4c, int scaleX, int scaleY,
                                      int colour, int unk_56, int unk_59,
                                      int side);

/* Starts the effect sprite of one HP cell: its motion starts at the cell's position, offset and
 * moving by the side's effect values scaled by the cell's scale, and waits the effect's delay.
 * A cell whose second position is entry 3 makes the sprite with that second position. */
void Battle_StartCellEffect(int side, int cell) {
  BattleCellTask* cellTask =
      (BattleCellTask*)MENU->battle.unk_310[side][cell].unk_00;
  int taskSide;

  MENU->battle.motions[side][cell].x = cellTask->posX;
  MENU->battle.motions[side][cell].y = cellTask->posY;
  MENU->battle.motions[side][cell].speedX =
      (cellTask->scale * SIDE_EFFECT(side)->offset.x) >> 8;
  MENU->battle.motions[side][cell].speedY =
      cellTask->angle + ((cellTask->scale * SIDE_EFFECT(side)->offset.y) >> 8);
  if (cellTask->pos2Index == 3) {
    taskSide = cellTask->side;
    MENU->battle.motions[side][cell].unk_00 = (s32)BattleSpriteTask_Create(
        (const u32*)MENU->battle.unk_48[taskSide], 0,
        MENU->battle.motions[taskSide][cell].x,
        MENU->battle.motions[taskSide][cell].y, cellTask->posZ,
        MENU->battle.motions[taskSide][cell].speedX,
        MENU->battle.motions[taskSide][cell].speedY, -1, cellTask->scale,
        cellTask->scale, (&MENU->colours[8])[taskSide], 8, 0, taskSide,
        cellTask->hasPos2, cellTask->pos2X, cellTask->pos2Y, cellTask->pos2Z,
        8);
  } else {
    taskSide = cellTask->side;
    MENU->battle.motions[side][cell].unk_00 = (s32)func_ov002_02356604(
        (const u32*)MENU->battle.unk_48[taskSide], 0,
        MENU->battle.motions[taskSide][cell].x,
        MENU->battle.motions[taskSide][cell].y, cellTask->posZ,
        MENU->battle.motions[taskSide][cell].speedX,
        MENU->battle.motions[taskSide][cell].speedY, -1, cellTask->scale,
        cellTask->scale, (&MENU->colours[8])[taskSide], 8, 0, taskSide);
  }
  MENU->battle.motions[side][cell].velocityX =
      (cellTask->scale * SIDE_EFFECT(side)->velocity.x) >> 8;
  MENU->battle.motions[side][cell].velocityY =
      (cellTask->scale * SIDE_EFFECT(side)->velocity.y) >> 8;
  MENU->battle.motions[side][cell].delay = SIDE_EFFECT(side)->delay;
}
