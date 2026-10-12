#include "types.h"
#include "game/camera_pan.h"
#include "game/task.h"

extern Camera data_022a7d5c;

int Math_Ease(int curve, int from, int to, int elapsed, int duration);
void Task_ResumeScript(Task* task);

/* Like CameraPan_UpdateCurve5, but with Math_Ease curve 4 and the pan's
 * positions swapped: it eases from `to` back to `from`. */
void CameraPan_UpdateCurve4Reverse(CameraPanTask* pan) {
  s32* from = pan->from;
  s32* to = pan->to;
  int x;
  int y;
  int z;

  if (pan->frame >= pan->frames) {
    Task_ResumeScript((Task*)pan);
    return;
  }
  x = Math_Ease(4, to[0], from[0], pan->frame, pan->frames);
  y = Math_Ease(4, to[1], from[1], pan->frame, pan->frames);
  z = Math_Ease(4, to[2], from[2], pan->frame, pan->frames);
  data_022a7d5c.x = x;
  data_022a7d5c.y = y;
  data_022a7d5c.z = z;
  if (pan->frame < pan->frames) {
    pan->frame++;
  }
}
