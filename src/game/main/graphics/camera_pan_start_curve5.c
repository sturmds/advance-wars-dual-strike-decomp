#include "types.h"
#include "game/camera_pan.h"
#include "game/task.h"

extern const TaskScriptCmd data_0216a23c[];

void Sound_PlaySe(int se);

/* Plays sound effect 156 and starts a curve 5 camera pan (script
 * data_0216a23c, see CameraPan_UpdateCurve5) as a child of parent. */
void CameraPan_StartCurve5(int frames, Task* parent) {
  Task* task;

  Sound_PlaySe(156);
  task = Task_CreateChild(data_0216a23c, parent);
  ((CameraPanTask*)task)->frames = frames;
}
