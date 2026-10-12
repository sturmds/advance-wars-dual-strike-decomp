#include "types.h"
#include "game/capture_picture_task.h"
#include "game/task.h"

extern const TaskScriptCmd data_02168df0[];

CapturePictureTask* Task_FindByScript(const TaskScriptCmd* script);
void Task_ResumeScript(void* task);

/* Frame step: shakes the capture picture for 8 frames by moving the sprites
 * in armySprite and propertySprite one pixel on odd frames (unk_42), then puts them back
 * and resumes its own script. */
void CaptureShake_Update(CaptureCountTask* task) {
  CapturePictureTask* picture = Task_FindByScript(data_02168df0);

  task->count++;
  if (task->count <= 8) {
    picture->unk_42 = task->count & 1;
    return;
  }
  picture->unk_42 = 0;
  Task_ResumeScript(task);
}
