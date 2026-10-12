#include "types.h"
#include "game/event_script.h"

void Sound_PlaySe(int seId);

/* Script command: plays the sound effect in arg. */
BOOL EventScript_PlaySe(s16 index) {
  Sound_PlaySe(data_022977f0[index].pc->arg);
  data_022977f0[index].pc++;
  return FALSE;
}
