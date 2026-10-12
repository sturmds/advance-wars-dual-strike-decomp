#include "types.h"
#include "game/Unk_0201d320.h"
#include "game/map_unit.h"

extern Unk_0201d320 data_022977f4[];

MapUnit* Unit_SpawnWaited(s16 x, s16 y, s16 unitType);

/* Event script command: creates a unit of type unk_0c at tile (unk_08, unk_0a)
 * and marks it as waited, then moves to the next record and ends the slot's
 * run for this frame. */
BOOL EventCmd_CreateUnit(s16 index) {
  Unk_0201d320_Entry* entry = data_022977f4[index].unk_00;

  Unit_SpawnWaited(entry->unk_08, entry->unk_0a, entry->unk_0c);
  data_022977f4[index].unk_00++;
  return FALSE;
}
