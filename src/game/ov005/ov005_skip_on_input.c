#include "types.h"
#include "game/ov005_task.h"
#include "game/Unk_ov005_0235a30c.h"

BOOL GameFlag_IsSet(u32 flag);
BOOL Touch_IsPressed(void);
BOOL Keypad_ConfirmPressed(void);

/* With game flag 57 set, a touch or a press of A or Start in one of the
 * skippable steps remembers that step and jumps the task to step 100. */
void Ov005_SkipOnInput(Unk_ov005_0235a30c* work, Ov005Task* task) {
  if (!GameFlag_IsSet(57)) {
    return;
  }
  if (!Touch_IsPressed() && !Keypad_ConfirmPressed()) {
    return;
  }
  switch (task->step) {
    case 10:
    case 11:
    case 20:
    case 60:
    case 61:
    case 62:
    case 70:
    case 71:
    case 80:
    case 81:
    case 82:
    case 90:
      work->skippedStep = task->step;
      task->step = 100;
      break;
  }
}
