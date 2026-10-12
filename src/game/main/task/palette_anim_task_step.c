#include "types.h"
#include "game/task.h"

void Task_ResumeScript(Task* task);
void PaletteAnimTask_LoadFrame(Task* task);

/* Script step: advances the frame counter unk_64 (PaletteAnimTask_LoadFrame advances it
 * too), loads palette frame (unk_64 >> 2) & 15, and ends the step once that
 * frame wraps to 0. The counter is read as signed to match. */
void PaletteAnimTask_Step(Task* task) {
  task->unk_64 = task->unk_64 + 1;
  PaletteAnimTask_LoadFrame(task);
  if ((task->unk_64 >> 2) & 0xf) {
    return;
  }
  Task_ResumeScript(task);
}
