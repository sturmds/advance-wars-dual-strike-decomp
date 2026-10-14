#include "types.h"
#include "game/BattleCellTask.h"
#include "game/BattleSideData.h"

#define MENU ((MenuState*)&gMenuState)

/* x of the offset of a cell's object (number -1: none), scaled by the task's scale. With
 * the side's unk_48 at 0 the shared offset and the cell's are added; otherwise the entry is
 * read from BattleFighterLayout.offsets. The callers pass it before the position's angle,
 * and the y after it. */
int BattleCellTask_GetCellOffsetX(BattleCellTask* task, int number) {
  BattleSideData* data = MENU->battle.unk_27c[task->side];
  const BattleLayout* layout;

  if (data->unk_48 == 0) {
    layout = data->layout;
    if (number == -1) {
      return layout->cell.base.x * task->scale >> 8;
    }
    return task->scale *
               (layout->cell.base.x + layout->cell.unitOffsets[number].x) >>
           8;
  }
  layout = data->layout;
  if (number == -1) {
    number = 0;
  }
  return task->scale * layout->fighter.offsets[number].x >> 8;
}
