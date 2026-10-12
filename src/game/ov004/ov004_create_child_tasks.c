#include "types.h"
#include "game/task.h"
#include "game/Unk_ov004_022bf158.h"

extern TaskScriptCmd data_ov004_022bf014[]; /* script of each child task */

#define CHILD(i) ((Ov004Task*)state->childTasks[i])

/* from and to look like a start x and y: -16 is just off the left edge, 192
 * the bottom edge of the screen (guess). */
#define SET_CHILD(i, from, to, frames, b48, b4c, b50) \
  CHILD(i)->slot = i;                                 \
  CHILD(i)->unk_38 = 0;                               \
  CHILD(i)->tweenFrom = from;                         \
  CHILD(i)->tweenTo = to;                             \
  CHILD(i)->tweenFrames = frames;                     \
  CHILD(i)->unk_48 = b48;                             \
  CHILD(i)->unk_4c = b4c;                             \
  CHILD(i)->unk_50 = b50

/* Creates the 16 tasks of state->childTasks (all on script
 * data_ov004_022bf014), then gives each its slot and starting values: ten
 * with unk_50 = 0 and unk_4c 0 or 1, six with unk_50 = 15 and unk_4c 2 or
 * 3. Ov004_DestroyChildTasks removes them again. */
void Ov004_CreateChildTasks(Unk_ov004_022bf158* state) {
  int i;
  Ov004Task* task;

  for (i = 0; i < 16; i++) {
    task = (Ov004Task*)Task_Create(data_ov004_022bf014, TASK_LIST(4));
    state->childTasks[i] = (struct Task*)task;
    task->slot = -1;
  }

  SET_CHILD(0, -16, 96, 680, 45, 1, 0);
  SET_CHILD(1, -16, 120, 630, 34, 1, 0);
  SET_CHILD(2, -16, 144, 806, 41, 0, 0);
  SET_CHILD(3, -16, 168, 736, 43, 0, 0);
  SET_CHILD(4, -16, 192, 860, 30, 1, 0);
  SET_CHILD(5, 32, 192, 840, 32, 1, 0);
  SET_CHILD(6, 56, 192, 900, 47, 0, 0);
  SET_CHILD(7, 80, 192, 640, 28, 1, 0);
  SET_CHILD(8, 104, 192, 780, 39, 0, 0);
  SET_CHILD(9, 128, 192, 980, 36, 1, 0);
  SET_CHILD(10, -16, 80, 500, 34, 3, 15);
  SET_CHILD(11, -16, 128, 370, 32, 3, 15);
  SET_CHILD(12, -16, 176, 450, 30, 3, 15);
  SET_CHILD(13, 48, 192, 350, 46, 2, 15);
  SET_CHILD(14, 88, 192, 480, 50, 2, 15);
  SET_CHILD(15, 128, 192, 420, 48, 2, 15);
}
