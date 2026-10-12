#include "types.h"
#include "game/Unk_0201d320.h"

extern Unk_0201d320 data_022977f4[];

/* Event script command: continues at the record the current record points to. */
BOOL EventCmd_Jump(s16 index) {
  data_022977f4[index].unk_00 = data_022977f4[index].unk_00->unk_04.jump;
  return 1;
}
