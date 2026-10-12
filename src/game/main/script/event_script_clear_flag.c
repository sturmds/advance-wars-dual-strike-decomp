#include "types.h"
#include "game/event_script.h"
#include "game/unlocks.h"

/* Script command: clears game flag unk_08. */
BOOL EventScript_ClearFlag(s16 index) {
  GameFlag_Set(data_022977f0[index].pc->unk_08, 0);
  data_022977f0[index].pc++;
  return TRUE;
}
