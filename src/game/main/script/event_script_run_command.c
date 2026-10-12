#include "types.h"
#include "game/event_script.h"

/* Runs the command at the slot's pc through its handler, then moves to the next
 * command. Returns whether the slot still has a script. */
s16 EventScript_RunCommand(s16 index) {
  data_022977f0[index].pc->handler(&data_022977f0[index]);
  data_022977f0[index].pc++;
  return data_022977f0[index].start != NULL;
}
