#include "types.h"
#include "game/ov003_unit.h"

extern Ov003Unit data_ov003_02369538[];
extern u8 data_ov003_02364d8e[][20];

int func_ov003_023622b8(void);
int Ov003_FindOpenTileNear(int tile, int* x, int* y);

#define STATE data_ov003_02364928

/* Creates the unit for the next spawn record of the current row and links
 * its cell to it. Returns NULL when the row has no records left or no entry
 * from func_ov003_023622b8() up is free (hp 0 and unk_17_7 clear), or when
 * Ov003_FindOpenTileNear rejects the position. Afterwards the record index moves
 * on; on row 5 it wraps to 0 at the row's spawnCount. */
Ov003Unit* Ov003Unit_SpawnNext(void) {
  Ov003Unit* unit;
  Ov003Row* row;
  Ov003Cell* cell;
  Ov003Spawn* spawn;
  int slot;
  int next;
  int x;
  int y;

  unit = data_ov003_02369538;
  next = STATE->nextSpawn;
  row = &STATE->rows[STATE->row - 1];
  if (next == row->spawnCount) {
    return NULL;
  }
  spawn = &STATE->spawns[next];
  cell = &row->cells[next];
  unit += func_ov003_023622b8();
  for (slot = func_ov003_023622b8(); slot < OV003_UNIT_COUNT; slot++, unit++) {
    if (unit->hp == 0 && !unit->unk_17_7) {
      break;
    }
  }
  if (slot == 16) {
    return NULL;
  }
  x = spawn->x;
  y = spawn->y;
  if (Ov003_FindOpenTileNear(cell->kind, &x, &y) == 0) {
    return NULL;
  }
  unit->x = (x << 7) + 64;
  unit->y = (y << 7) + 64;
  unit->facing = spawn->unk_2_0;
  unit->kind = cell->kind;
  unit->row = STATE->row;
  unit->unk_15 = 0;
  unit->hp = 100;
  unit->unk_17_6 = 0;
  unit->unk_17_7 = 0;
  unit->unk_04 = 0;
  unit->unk_11 = 0;
  unit->walkCounter = 0;
  unit->unk_17_0 = 0;
  unit->safeTimer = 0;
  unit->aiTimer = ((unit - data_ov003_02369538) & 15) << 4;
  unit->shotWait = data_ov003_02364d8e[unit->kind][0];
  unit->shotCount = 0;
  unit->scatterTarget = -1;
  unit->target = -1;
  unit->unk_1d_6 = spawn->unk_2_7;
  unit->unk_1d_7 = spawn->unk_2_6;
  unit->awake = spawn->unk_2_3;
  unit->wakeRange = spawn->unk_3;
  unit->wakeGroup = spawn->unk_4;
  unit->prevX = unit->x;
  unit->prevY = unit->x;
  unit->prevUnk_04 = unit->unk_04;
  unit->prevHp = unit->hp;
  if (STATE->row != 5) {
    cell->state = 2;
  } else {
    cell->state = 1;
  }
  cell->unit = slot;
  STATE->nextSpawn++;
  if (STATE->row == 5) {
    row = &STATE->rows[STATE->row - 1];
    if (STATE->nextSpawn == row->spawnCount) {
      STATE->nextSpawn = 0;
    }
  }
  return unit;
}
