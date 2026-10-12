#include "types.h"
#include "game/BattleCellTask.h"
#include "game/BattleSideData.h"

#define MENU ((MenuState*)&gMenuState)

/* Vertical twin of BattleCellTask_GetCellOffsetX. */
int BattleCellTask_GetCellOffsetY(BattleCellTask* task, int number) {
  BattleSideData* data = MENU->battle.unk_27c[task->side];
  BattleLayout* layout;

  if (data->unk_48 == 0) {
    layout = (BattleLayout*)data->cellPositions;
    if (number == -1) {
      return layout->base.y * task->scale >> 8;
    }
    return task->scale * (layout->base.y + layout->unitOffsets[number].y) >> 8;
  }
  layout = (BattleLayout*)data->cellPositions;
  if (number == -1) {
    number = 0;
  }
  return task->scale * (&layout->base)[number].y >> 8;
}
