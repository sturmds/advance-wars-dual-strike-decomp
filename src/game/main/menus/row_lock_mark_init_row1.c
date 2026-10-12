#include "types.h"
#include "game/row_lock_mark_task.h"

/* First step of the row 1 lock mark script: the mark sits at (224, 56)
 * and uses affine matrix 25. */
void RowLockMark_InitRow1(RowLockMarkTask* task) {
  task->frame = 0;
  task->affineIndex = 25;
  task->x = 224;
  task->y = 56;
}
