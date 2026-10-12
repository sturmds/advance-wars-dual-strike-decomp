#include "types.h"
#include "game/task.h"
#include "game/ov005_slide_task.h"

extern const TaskScriptCmd data_ov005_023695f4[];

/* Starts a task that moves entry `index` of list B up by 384 over 384 frames. */
Task* Ov005Slide_StartEntryB(Unk_ov005_0236bb90* work, int index) {
  Ov005SlideTask* slide =
      (Ov005SlideTask*)Task_Create(data_ov005_023695f4, TASK_LIST(3));

  slide->work = work;
  slide->index = index;
  slide->resumeScript = 0;
  slide->entries = work->entriesB;
  slide->from = work->entriesB[index].pos;
  slide->to = work->entriesB[index].pos - 384;
  slide->frame = 0;
  slide->frames = 384;
  slide->step = 0;
  slide->busy = 1;
  return (Task*)slide;
}
