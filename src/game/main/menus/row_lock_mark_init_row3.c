#include "types.h"
#include "game/row_lock_mark_task.h"

/* First step of the row 3 lock mark script: the mark sits at (224, 120)
 * and uses affine matrix 27. */
void RowLockMark_InitRow3(RowLockMarkTask* task) {
  task->frame = 0;
  task->affineIndex = 27;
  task->x = 224;
  task->y = 120;
}
