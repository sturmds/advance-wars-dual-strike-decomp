#include "types.h"
#include "game/event_script.h"
#include "game/task.h"

/* Event script command: starts a task from the script at +4 with the command's
 * value as parent or list (see Task_Create), then moves to the next command. */
BOOL EventScript_StartTask(s16 index) {
  Task_Create(EVENT_SCRIPT_DATA_COMMAND(index)->target,
              (void*)EVENT_SCRIPT_DATA_COMMAND(index)->value);
  data_022977f0[index].pc++;
  return TRUE;
}
