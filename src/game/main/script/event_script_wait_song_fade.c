#include "types.h"
#include "game/event_script.h"

void func_0201b130(EventScriptSlot* slot);

/* Script command: sets hold to func_0201b130, which clears it once no song is
 * fading out. hold is a u32 in EventScriptSlot, so the address is cast. */
BOOL EventScript_WaitSongFade(s16 index) {
  data_022977f0[index].hold = (u32)func_0201b130;
  data_022977f0[index].pc++;
  return FALSE;
}
