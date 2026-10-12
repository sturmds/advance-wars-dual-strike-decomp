#include "types.h"
#include "game/Unk_0201d320.h"
#include "game/unlocks.h"

extern Unk_0201d320 data_022977f4[];

BOOL EventCmd_Jump(s16 index);

/* Event script command: goes on with the next record when game flag unk_08 is
 * clear, otherwise jumps to the record in unk_04. */
BOOL EventCmd_JumpIfFlagSet(s16 index) {
  Unk_0201d320* script = &data_022977f4[index];

  if (!GameFlag_IsSet(script->unk_00->unk_08)) {
    script->unk_00++;
    return TRUE;
  }
  return EventCmd_Jump(index);
}
