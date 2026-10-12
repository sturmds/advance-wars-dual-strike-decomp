#include "types.h"
#include "game/event_script.h"
#include "game/task.h"

Task* Task_FindByScript(const void* script);

/* Event script command: waits until no task runs the script at +4. Returns
 * FALSE (and stays on the command) while one does. */
BOOL EventScript_WaitTask(s16 index) {
  if (Task_FindByScript(EVENT_SCRIPT_DATA_COMMAND(index)->target) != NULL) {
    return FALSE;
  }
  data_022977f0[index].pc++;
  return TRUE;
}
