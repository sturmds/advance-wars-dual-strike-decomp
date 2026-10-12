#include "types.h"
#include "game/row_lock_mark_task.h"

/* First step of the row 4 lock mark script: the mark sits at (224, 152)
 * and uses affine matrix 28. */
void RowLockMark_InitRow4(RowLockMarkTask* task) {
  task->frame = 0;
  task->affineIndex = 28;
  task->x = 224;
  task->y = 152;
}
