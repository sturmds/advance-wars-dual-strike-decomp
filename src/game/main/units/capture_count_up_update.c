#include "types.h"
#include "game/capture_picture_task.h"
#include "game/task.h"

extern const TaskScriptCmd data_02168df0[];

CapturePictureTask* Task_FindByScript(const TaskScriptCmd* script);
void Task_ResumeScript(void* task);

/* Frame step: raises the capture points shown by the picture task one per
 * frame until count reaches limit, then resumes its own script. */
void CaptureCountUp_Update(CaptureCountTask* task) {
  CapturePictureTask* picture = Task_FindByScript(data_02168df0);

  if (task->count < task->limit) {
    task->count++;
    picture->points = task->count;
    return;
  }
  Task_ResumeScript(task);
}
