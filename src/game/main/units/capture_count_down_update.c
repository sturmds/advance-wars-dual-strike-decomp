#include "types.h"
#include "game/capture_picture_task.h"
#include "game/task.h"

extern const TaskScriptCmd data_02168df0[];

CapturePictureTask* Task_FindByScript(const TaskScriptCmd* script);
void Task_ResumeScript(void* task);
void SpriteAnim_SetAnimation(void* sprite, int animation);

/* Frame step: lowers the capture points shown by the picture task one per
 * frame. At 0 it resumes its own script and sets animation 3 on the
 * picture's armySprite. */
void CaptureCountDown_Update(CaptureCountTask* task) {
  CapturePictureTask* picture = Task_FindByScript(data_02168df0);

  if (task->count != 0) {
    task->count--;
    picture->points = task->count;
    return;
  }
  Task_ResumeScript(task);
  SpriteAnim_SetAnimation(picture->armySprite, 3);
}
