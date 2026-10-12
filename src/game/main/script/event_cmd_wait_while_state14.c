#include "types.h"
#include "game/Unk_02080440.h"
#include "game/Unk_0201d320.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;
extern Unk_0201d320 data_022977f4[];

#define STATE (data_027c027c->unk_48[data_027c0284])

/* Event script command: stays on this record, ending the slot's run each
 * frame, while the game state (unk_18) is 14. */
BOOL EventCmd_WaitWhileState14(s16 index) {
  if (STATE->unk_18 == EVENT_GAME_STATE_14) {
    return 0;
  }
  data_022977f4[index].unk_00++;
  return 1;
}
