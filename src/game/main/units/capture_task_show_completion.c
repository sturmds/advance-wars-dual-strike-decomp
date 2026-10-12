#include "types.h"
#include "game/capture_picture_task.h"
#include "game/task.h"

extern const TaskScriptCmd data_02168d48[];
extern const TaskScriptCmd data_02168ea8[];

int Capture_PlayCompleteSound(int owner, int tile);
void PropertyPicture_Load(int tile, int gfxTile, int palette);
void Sound_PlaySe(u32 soundId);
int Task_JumpToLabel(Task* task, int label);

/* Script step of the capture task (list 3). Below 20 points it jumps the
 * script to label 0. At 20 it calls Capture_PlayCompleteSound, plays sound
 * 150 when that returns 0, loads the property picture to sprite tile 500,
 * palette 5, and starts two tasks under this one: script data_02168d48 and
 * the CaptureDone task at the same position. */
void CaptureTask_ShowCompletion(CapturePictureTask* task) {
  CaptureDoneTask* done;

  if (task->newPoints < 20) {
    Task_JumpToLabel((Task*)task, 0);
    return;
  }
  if (Capture_PlayCompleteSound(task->owner, task->tile) == 0) {
    Sound_PlaySe(150);
  }
  PropertyPicture_Load(task->tile, 500, 5);
  Task_CreateChild(data_02168d48, (Task*)task);
  done = (CaptureDoneTask*)Task_Create(data_02168ea8, task);
  done->x = task->x;
  done->y = task->y;
}
