#include "types.h"
#include "game/event_script.h"

/* Event script command: copies the byte at the address in the command's value to the address at +4, then moves to the next command. */
BOOL EventScript_CopyByte(s16 index) {
  *(u8*)EVENT_SCRIPT_DATA_COMMAND(index)->target =
      *(u8*)EVENT_SCRIPT_DATA_COMMAND(index)->value;
  data_022977f0[index].pc++;
  return TRUE;
}
