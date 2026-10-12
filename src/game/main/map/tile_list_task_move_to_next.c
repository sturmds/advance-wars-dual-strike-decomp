#include "types.h"
#include "game/Unk_02168140.h"
#include "game/tile_point.h"

int Task_JumpToLabel(Unk_02168140Task* task, int label);
void func_020f5888(int x, int y);

/* Probably a step of the data_02168140 script (same fields): unk_4c walks a list of tile
 * positions that ends at x == 0xffff. Each call takes the next tile, keeps it
 * in unk_2c and unk_30 and passes it to func_020f5888 (most callers use that
 * to move the view). At the end of the list the script jumps to label 1.
 * Name is a guess. */
void TileListTask_MoveToNext(Unk_02168140Task* task) {
  TilePoint* point = task->unk_4c;

  if ((u16)point->x == 0xffff) {
    Task_JumpToLabel(task, 1);
    return;
  }
  task->unk_2c = point->x;
  task->unk_30 = point->y;
  func_020f5888((s16)task->unk_2c, (s16)task->unk_30);
  task->unk_4c = point + 1;
}
