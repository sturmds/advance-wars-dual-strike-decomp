#include "types.h"
#include "game/Unk_02080440.h"
#include "game/Unk_0201d320.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;
extern Unk_0201d320 data_022977f4[];

#define STATE (data_027c027c->unk_48[data_027c0284])

void ScreenConfig_RunActionHook(void);

/* Event script command: sets the game state byte unk_84 (meaning unknown) to 1, runs the
 * screen config action hook, then moves to the next record. */
BOOL EventCmd_SetFlag84RunActionHook(s16 index) {
  STATE->unk_84 = 1;
  ScreenConfig_RunActionHook();
  data_022977f4[index].unk_00++;
  return 1;
}
