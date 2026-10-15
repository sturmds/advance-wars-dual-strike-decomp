#include "types.h"
#include "game/Unk_02080440.h"
#include "game/Unk_0201d320.h"

extern Unk_0201d320 data_022977f4[];

/* Event script command: waits on this record, ending the slot's run each
 * frame, until the game state (unk_18) is 14. */
BOOL EventCmd_WaitForState14(s16 index) {
  if (MAP_STATE->unk_18 != EVENT_GAME_STATE_14) {
    return 0;
  }
  data_022977f4[index].unk_00++;
  return 1;
}
