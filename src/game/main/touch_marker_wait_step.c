#include "types.h"
#include "game/touch_marker_task.h"

void Task_ResumeScript(TouchMarkerTask* task);

/* Script step of the touch marker task: counts frames, then lets the script
 * go on. */
void TouchMarker_WaitStep(TouchMarkerTask* task) {
  if (task->age >= TOUCH_MARKER_FRAMES) {
    Task_ResumeScript(task);
    return;
  }
  /* needed to match: the original tests the count a second time */
  if (task->age < TOUCH_MARKER_FRAMES) {
    task->age++;
  }
}
