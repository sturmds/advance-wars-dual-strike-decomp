#include "types.h"
#include "game/ov003_unit.h"

void Ov003Row_AppendCell(int row, int tile, int state);

/* Copies a spawn list into Ov003State.spawns and marks a cell in the current
 * row for each record. */
void Ov003_LoadSpawns(Ov003SpawnDef* defs) {
  Ov003Spawn* spawn = &data_ov003_02364928->spawns[0];

  if (defs->tag == 0) {
    return;
  }
  do {
    Ov003Row_AppendCell(data_ov003_02364928->row, defs->tag, 1);
    spawn->x = defs->x;
    spawn->y = defs->y;
    spawn->unk_2_0 = defs->flags_0;
    spawn->unk_2_3 = defs->flags_3;
    spawn->unk_2_6 = defs->flags_6;
    spawn->unk_2_7 = defs->flags_7;
    spawn->unk_3 = defs->unk_4;
    spawn->unk_4 = defs->unk_5;
    defs++;
    spawn++;
  } while (defs->tag != 0);
}
