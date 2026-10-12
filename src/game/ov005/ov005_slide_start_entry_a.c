#include "types.h"
#include "game/task.h"
#include "game/ov005_slide_task.h"

extern const TaskScriptCmd data_ov005_023695f4[];

/* Starts a task that moves entry `index` of list A up by 256 over 256 frames. */
Task* Ov005Slide_StartEntryA(Unk_ov005_0236bb90* work, int index) {
  Ov005SlideTask* slide =
      (Ov005SlideTask*)Task_Create(data_ov005_023695f4, TASK_LIST(3));

  slide->work = work;
  slide->index = index;
  slide->resumeScript = 0;
  slide->entries = work->entriesA;
  slide->from = work->entriesA[index].pos;
  slide->to = work->entriesA[index].pos - 256;
  slide->frame = 0;
  slide->frames = 256;
  slide->step = 0;
  slide->busy = 1;
  return (Task*)slide;
}
