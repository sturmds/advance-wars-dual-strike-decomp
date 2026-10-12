#include "types.h"
#include "game/BattleCellTask.h"
#include "game/BattleSideData.h"

#define MENU ((MenuState*)&gMenuState)

/* Horizontal offset of a cell's object (number -1: none), scaled by the task's scale. With
 * the side's unk_48 at 0 the shared offset and the cell's are added; otherwise the entry is
 * read from the 6-entry table that starts with the shared offset. */
int BattleCellTask_GetCellOffsetX(BattleCellTask* task, int number) {
  BattleSideData* data = MENU->battle.unk_27c[task->side];
  BattleLayout* layout;

  if (data->unk_48 == 0) {
    layout = (BattleLayout*)data->cellPositions;
    if (number == -1) {
      return layout->base.x * task->scale >> 8;
    }
    return task->scale * (layout->base.x + layout->unitOffsets[number].x) >> 8;
  }
  layout = (BattleLayout*)data->cellPositions;
  if (number == -1) {
    number = 0;
  }
  return task->scale * (&layout->base)[number].x >> 8;
}
