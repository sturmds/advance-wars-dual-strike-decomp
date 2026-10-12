#include "types.h"
#include "game/event_script.h"

extern u8 gEventSkipState;
void EventScript_SetArmyPalette(int army);

/* Script command: loads the palette of the army in unk_08. When gEventSkipState is 1
 * the command only advances. */
BOOL EventScript_SetPalette(s16 index) {
  s16 army = data_022977f0[index].pc->unk_08;

  if (gEventSkipState != 1) {
    EventScript_SetArmyPalette(army);
  }
  data_022977f0[index].pc++;
  return TRUE;
}
