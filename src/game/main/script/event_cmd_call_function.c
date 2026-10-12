#include "types.h"
#include "game/Unk_0201d320.h"
#include "game/event_script.h"

extern Unk_0201d320 data_022977f4[];

/* Event script command: calls the record's function with the record's
 * argument, then moves to the next record. Returns whether the slot still
 * has a script: the function may have stopped it. */
s16 EventCmd_CallFunction(s16 index) {
  Unk_0201d320* script = &data_022977f4[index];

  script->unk_00->unk_04.run(script->unk_00->unk_0c);
  script->unk_00++;
  return data_022977f0[index].start != NULL;
}
