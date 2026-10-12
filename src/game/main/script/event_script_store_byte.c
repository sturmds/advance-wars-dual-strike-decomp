#include "types.h"
#include "game/event_script.h"

/* Event script command: stores the low byte of the command's value at the address at +4, then moves to the next command. */
BOOL EventScript_StoreByte(s16 index) {
  *(u8*)EVENT_SCRIPT_DATA_COMMAND(index)->target =
      EVENT_SCRIPT_DATA_COMMAND(index)->value;
  data_022977f0[index].pc++;
  return TRUE;
}
