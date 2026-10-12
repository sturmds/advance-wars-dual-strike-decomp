#include "types.h"
#include "game/event_script.h"

/* Event script command: stores the command's value as a word at the address at +4, then moves to the next command. */
BOOL EventScript_StoreWord(s16 index) {
  *(u32*)EVENT_SCRIPT_DATA_COMMAND(index)->target =
      EVENT_SCRIPT_DATA_COMMAND(index)->value;
  data_022977f0[index].pc++;
  return TRUE;
}
