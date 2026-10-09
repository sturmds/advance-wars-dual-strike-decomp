#include "types.h"
#include "game/ov003_unit.h"

/* Five rows of 31 cells (62 bytes) in data_ov003_02364928. The cell at index 29
 * holds the number of spawn records the row uses (read as a plain byte). */
typedef struct Ov003Row {
    Ov003Cell cells[29];
    u8 spawnCount;
    u8 pad_3b[3];
} Ov003Row;

/* 5 bytes at Ov003State.spawns: where a unit appears and how it starts. */
typedef struct Ov003Spawn {
    u8 x;                              /* passed to func_ov003_0235c42c */
    u8 y;
    u8 unk_2_0 : 3;                    /* becomes Ov003Unit.unk_12_0 */
    u8 unk_2_3 : 3;                    /* bit 0 becomes Ov003Unit.unk_1e_0 */
    u8 unk_2_6 : 1;                    /* becomes Ov003Unit.unk_1d_7 */
    u8 unk_2_7 : 1;                    /* becomes Ov003Unit.unk_1d_6 */
    u8 unk_3;                          /* becomes Ov003Unit.unk_1f */
    u8 unk_4;                          /* becomes Ov003Unit.unk_1e_1 */
} Ov003Spawn;

typedef struct Ov003State {
    Ov003Row rows[5];
    u8 pad_136[0x156 - 0x136];
    Ov003Spawn spawns[1];              /* length unknown */
    u8 pad_15b[0x1db - 0x15b];
    u8 row : 7;                        /* 1 to 5: the row spawns go to */
    u8 unk_1db_7 : 1;
    u8 pad_1dc;
    u8 nextSpawn;                      /* index of the next record in spawns and cell in the row */
} Ov003State;

extern Ov003State *data_ov003_02364928;
extern Ov003Unit data_ov003_02369538[16];
extern u8 data_ov003_02364d8e[][20];

int func_ov003_023622b8(void);
int func_ov003_0235c42c(int tile, int *x, int *y);

#define STATE data_ov003_02364928

/* Creates the unit for the next spawn record of the current row and links
 * its cell to it. Returns NULL when the row has no records left or no entry
 * from func_ov003_023622b8() up is free (hp 0 and unk_17_7 clear), or when
 * func_ov003_0235c42c rejects the position. Afterwards the record index moves
 * on; on row 5 it wraps to 0 at the row's spawnCount. */
Ov003Unit *Ov003Unit_SpawnNext(void)
{
    Ov003Unit *unit;
    Ov003Row *row;
    Ov003Cell *cell;
    Ov003Spawn *spawn;
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
    for (slot = func_ov003_023622b8(); slot < 16; slot++, unit++) {
        if (unit->hp == 0 && !unit->unk_17_7) {
            break;
        }
    }
    if (slot == 16) {
        return NULL;
    }
    x = spawn->x;
    y = spawn->y;
    if (func_ov003_0235c42c(cell->tile, &x, &y) == 0) {
        return NULL;
    }
    unit->x = (x << 7) + 64;
    unit->y = (y << 7) + 64;
    unit->unk_12_0 = spawn->unk_2_0;
    unit->tile = cell->tile;
    unit->unk_12_3 = STATE->row;
    unit->unk_15 = 0;
    unit->hp = 100;
    unit->unk_17_6 = 0;
    unit->unk_17_7 = 0;
    unit->unk_04 = 0;
    unit->unk_11 = 0;
    unit->unk_16 = 0;
    unit->unk_17_0 = 0;
    unit->unk_08 = 0;
    unit->unk_18 = ((unit - data_ov003_02369538) & 15) << 4;
    unit->unk_19 = data_ov003_02364d8e[unit->tile][0];
    unit->unk_1d_0 = 0;
    unit->unk_1b = -1;
    unit->unk_1a = -1;
    unit->unk_1d_6 = spawn->unk_2_7;
    unit->unk_1d_7 = spawn->unk_2_6;
    unit->unk_1e_0 = spawn->unk_2_3;
    unit->unk_1f = spawn->unk_3;
    unit->unk_1e_1 = spawn->unk_4;
    unit->startX = unit->x;
    unit->unk_0c = unit->x;
    unit->startUnk_04 = unit->unk_04;
    unit->startHp = unit->hp;
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
