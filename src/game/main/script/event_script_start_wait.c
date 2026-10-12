#include "types.h"
#include "game/event_script.h"

/* Wait command: loads the slot's frame counter from the command and moves
 * to the next command. func_0201d320 calls it unless gEventSkipState is 1. */
BOOL EventScript_StartWait(s16 index) {
  data_022977f0[index].wait = data_022977f0[index].pc->arg;
  data_022977f0[index].pc++;
  return FALSE;
}
