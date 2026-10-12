#include "types.h"
#include "game/Unk_02080440.h"
#include "game/Unk_0201d320.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;
extern Unk_0201d320 data_022977f4[];

u16 Map_FindIdByData(const void* mapData);

#define STATE (data_027c027c->unk_48[data_027c0284])

/* Event script command: sets the current map id to the map whose data the
 * record points at (Map_FindIdByData), then moves to the next record and ends
 * the slot's run for this frame. */
BOOL EventCmd_SetMapId(s16 index) {
  STATE->unk_1c4->mapId =
      Map_FindIdByData(data_022977f4[index].unk_00->unk_04.jump);
  data_022977f4[index].unk_00++;
  return FALSE;
}
