#include "types.h"
#include "game/event_script.h"
#include "game/unlocks.h"

/* Script command: sets game flag unk_08. */
BOOL EventScript_SetFlag(s16 index) {
  GameFlag_Set(data_022977f0[index].pc->unk_08, 1);
  data_022977f0[index].pc++;
  return TRUE;
}
