#include "types.h"
#include "game/row_lock_mark_task.h"

/* First step of the row 2 lock mark script: the mark sits at (224, 88)
 * and uses affine matrix 26. */
void RowLockMark_InitRow2(RowLockMarkTask* task) {
  task->frame = 0;
  task->affineIndex = 26;
  task->x = 224;
  task->y = 88;
}
