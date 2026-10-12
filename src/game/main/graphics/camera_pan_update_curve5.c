#include "types.h"
#include "game/camera_pan.h"
#include "game/task.h"

extern Camera data_022a7d5c;

int Math_Ease(int curve, int from, int to, int elapsed, int duration);
void Task_ResumeScript(Task* task);

/* One frame of a camera pan (see CameraPan_InitFromScrollTile): sets the
 * camera position to the ease of curve 5 between the pan's two positions at
 * frame `frame` of `frames`; resumes the task's script once the pan is done. */
void CameraPan_UpdateCurve5(CameraPanTask* pan) {
  s32* from = pan->from;
  s32* to = pan->to;
  int x;
  int y;
  int z;

  if (pan->frame >= pan->frames) {
    Task_ResumeScript((Task*)pan);
    return;
  }
  x = Math_Ease(5, from[0], to[0], pan->frame, pan->frames);
  y = Math_Ease(5, from[1], to[1], pan->frame, pan->frames);
  z = Math_Ease(5, from[2], to[2], pan->frame, pan->frames);
  data_022a7d5c.x = x;
  data_022a7d5c.y = y;
  data_022a7d5c.z = z;
  if (pan->frame < pan->frames) {
    pan->frame++;
  }
}
