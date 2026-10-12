#include "types.h"
#include "game/Unk_02080440.h"
#include "game/Unk_0201d320.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;
extern Unk_0201d320 data_022977f4[];

#define STATE (data_027c027c->unk_48[data_027c0284])

/* Event script command: stores the record's value in unk_1c4->unk_37, a
 * flag byte that the link code tests bit by bit, then moves to the next
 * record. */
BOOL EventCmd_SetMapFlags(s16 index) {
  STATE->unk_1c4->unk_37 = data_022977f4[index].unk_00->unk_08;
  data_022977f4[index].unk_00++;
  return 1;
}
