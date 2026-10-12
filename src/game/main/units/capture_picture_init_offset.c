#include "types.h"
#include "game/capture_picture_task.h"
#include "game/task.h"

extern const TaskScriptCmd data_02168df0[];

CapturePictureTask* Task_FindByScript(const TaskScriptCmd* script);

/* Script step: sets the y offset CapturePicture_Draw gives armySprite
 * for the points the capture starts from. */
void CapturePicture_InitOffset(CapturePictureTask* task) {
  CapturePictureTask* picture = Task_FindByScript(data_02168df0);

  picture->unk_40 = -(20 - task->oldPoints) * 2;
}
