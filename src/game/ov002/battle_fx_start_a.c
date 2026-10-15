#include "types.h"
#include "game/BattleSideData.h"
#include "game/BattleSpriteTask.h"
#include "game/battle_scene.h"
#include "game/menu_state.h"

#define MENU ((MenuState*)&gMenuState)
/* Needed to match: the effect is looked up again for every field, not kept in a local. */
#define EFFECT                \
  (MENU->battle.unk_27c[side] \
       ->effects[Ov002_gBattleScene->sides[side].usesSecondary])

BattleSpriteTask* func_ov002_02356604(int table, int index, int x, int y,
                                      int unk_40, int offsetX, int offsetY,
                                      int unk_4c, int scaleX, int scaleY,
                                      int unk_54, int unk_56, int unk_59,
                                      int side);

/* Starts the motion of a cell from the side's first cell task: it starts at the task's
 * position, and its speed adds the cell's point and the side's effect offset, scaled by the
 * task's scale. The velocity and delay come from the side's effect. */
void Battle_StartCellEffectAtPoint(int side, int cell) {
  BattleSpriteTask* cellTask =
      (BattleSpriteTask*)MENU->battle.unk_310[side][0].unk_00;
  u8 taskSide;
  const BattleFighterLayout* points =
      &MENU->battle.unk_27c[side]->layout->fighter;

  MENU->battle.motions[side][cell].x = cellTask->unk_38;
  MENU->battle.motions[side][cell].y = cellTask->unk_3a;
  MENU->battle.motions[side][cell].speedX =
      (cellTask->scale * (points->steps[cell].x + EFFECT->offset.x)) >> 8;
  MENU->battle.motions[side][cell].speedY =
      cellTask->unk_46 +
      ((cellTask->scale * (points->steps[cell].y + EFFECT->offset.y)) >> 8);
  taskSide = cellTask->side;
  MENU->battle.motions[side][cell].unk_00 = (s32)func_ov002_02356604(
      MENU->battle.unk_48[taskSide], 0, MENU->battle.motions[taskSide][cell].x,
      MENU->battle.motions[taskSide][cell].y, cellTask->unk_3c,
      MENU->battle.motions[taskSide][cell].speedX,
      MENU->battle.motions[taskSide][cell].speedY, -1, cellTask->scale,
      cellTask->scale, (&MENU->colours[8])[taskSide], 8, 0, taskSide);
  MENU->battle.motions[side][cell].velocityX =
      (cellTask->scale * EFFECT->velocity.x) >> 8;
  MENU->battle.motions[side][cell].velocityY =
      (cellTask->scale * EFFECT->velocity.y) >> 8;
  MENU->battle.motions[side][cell].delay = EFFECT->delay;
}
