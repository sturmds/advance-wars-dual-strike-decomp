#include "types.h"
#include "game/camera_pan.h"
#include "game/task.h"

extern const TaskScriptCmd data_0216a21c[];

void Sound_PlaySe(int se);

/* Plays sound effect 155 and starts a camera pan with the curve 4 reverse
 * update (script data_0216a21c, see CameraPan_UpdateCurve4Reverse) as a
 * child of parent. */
void CameraPan_StartCurve4Reverse(int frames, Task* parent) {
  Task* task;

  Sound_PlaySe(155);
  task = Task_CreateChild(data_0216a21c, parent);
  ((CameraPanTask*)task)->frames = frames;
}
