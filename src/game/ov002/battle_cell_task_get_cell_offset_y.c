#include "types.h"
#include "game/BattleCellTask.h"
#include "game/BattleSideData.h"

#define MENU ((MenuState*)&gMenuState)

/* The y twin of BattleCellTask_GetCellOffsetX. */
int BattleCellTask_GetCellOffsetY(BattleCellTask* task, int number) {
  BattleSideData* data = MENU->battle.unk_27c[task->side];
  const BattleLayout* layout;

  if (data->unk_48 == 0) {
    layout = data->layout;
    if (number == -1) {
      return layout->cell.base.y * task->scale >> 8;
    }
    return task->scale *
               (layout->cell.base.y + layout->cell.unitOffsets[number].y) >>
           8;
  }
  layout = data->layout;
  if (number == -1) {
    number = 0;
  }
  return task->scale * layout->fighter.offsets[number].y >> 8;
}
