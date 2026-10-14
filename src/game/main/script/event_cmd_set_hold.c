#include "types.h"
#include "game/Unk_0201d320.h"

extern Unk_0201d320 data_022977f4[];
extern EventSlotHold data_022977f8[];

/* Event script command: sets the slot's hold to the record's function
 * pointer (nonzero, so the script stops advancing) and ends the slot's run
 * for this frame. */
BOOL EventCmd_SetHold(s16 index) {
  data_022977f8[index].hold = (u32)data_022977f4[index].unk_00->u.ptr;
  data_022977f4[index].unk_00++;
  return 0;
}
