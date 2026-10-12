#include "types.h"
#include "game/Unk_0201d320.h"
#include "game/event_script.h"

extern u8 gEventSkipState;
extern s16 data_02297780;
extern Unk_0201d320 data_022977f4[];
extern EventSlotHold data_022977f8[];
extern EventSlotCounter data_022977fe[];

void EventHold_StepScreenSlideIn(EventScriptSlot* slot);

/* Event script command: sets the hold function that steps the screen slide, from step 0,
 * then moves to the next record. Skipped when gEventSkipState is 1 and data_02297780 is
 * nonzero. "Slide" is a guess: the hold function moves two screen offsets from -112. */
BOOL EventCmd_StartScreenSlideIn(s16 index) {
  if (gEventSkipState != 1 || data_02297780 == 0) {
    data_022977fe[index].counter = 0;
    data_022977f8[index].hold = (u32)EventHold_StepScreenSlideIn;
  }
  data_022977f4[index].unk_00++;
  return 0;
}
