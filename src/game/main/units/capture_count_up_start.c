#include "types.h"
#include "game/capture_picture_task.h"
#include "game/task.h"

extern const TaskScriptCmd data_02168da8[];

/* Starts the point counter (script data_02168da8) under the picture task,
 * from oldPoints up to newPoints. */
void CaptureCountUp_Start(CapturePictureTask* task) {
  CaptureCountTask* count =
      (CaptureCountTask*)Task_CreateChild(data_02168da8, (Task*)task);

  count->count = task->oldPoints;
  count->limit = task->newPoints;
}
