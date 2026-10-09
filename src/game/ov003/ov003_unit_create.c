#include "types.h"
#include "game/ov003_unit.h"

typedef struct Ov003Slot {
    u8 pad_00;
    u8 row : 4;                        /* 1 + row of cells; bit 0 picks unk_12_0 */
    u8 pad_02[2];
} Ov003Slot;

typedef struct Ov003State {
    Ov003Cell cells[5][31];
    u8 pad_136[2];
    Ov003Slot slots[1];
} Ov003State;

extern Ov003State *data_ov003_02364928;
extern Ov003Unit data_ov003_02369538[16];

int func_ov003_023622b8(void);
int func_ov003_0235c42c(int tile, int *x, int *y);

/* Fills the Ov003Unit for slot with a unit at the cell column of its row in
 * data_ov003_02364928 and links the cell to it. A slot below
 * func_ov003_023622b8() is used as is; otherwise the first free entry from that
 * number up is taken. x and y go to func_ov003_0235c42c, which turns them into
 * the unit's tile position. Returns NULL when no entry is free or
 * func_ov003_0235c42c returns 0. Needed to match: row is a u8 and the
 * declarations are split from their assignments. */
Ov003Unit *Ov003Unit_Create(int slot, int column, int x, int y)
{
    u8 row;
    Ov003Unit *unit;
    Ov003Cell *cell;
    row = data_ov003_02364928->slots[slot].row;
    unit = data_ov003_02369538;
    cell = &data_ov003_02364928->cells[row - 1][column];

    if (slot < func_ov003_023622b8()) {
        unit += slot;
    } else {
        unit += func_ov003_023622b8();
        slot = func_ov003_023622b8();
        for (; slot < 16; slot++, unit++) {
            if (unit->hp == 0) {
                break;
            }
        }
        if (slot == 16) {
            return NULL;
        }
    }
    if (func_ov003_0235c42c(cell->tile, &x, &y) == 0) {
        return NULL;
    }
    unit->x = (x << 7) + 64;
    unit->y = (y << 7) + 64;
    if (row & 1) {
        unit->unk_12_0 = 4;
    } else {
        unit->unk_12_0 = 3;
    }
    unit->tile = cell->tile;
    unit->unk_12_3 = row;
    unit->unk_15 = 0;
    unit->hp = 100;
    unit->unk_17_6 = 0;
    unit->unk_17_7 = 0;
    unit->unk_04 = 0;
    unit->unk_11 = 0;
    unit->unk_16 = 0;
    unit->unk_17_0 = 0;
    unit->unk_08 = 0;
    unit->unk_19 = 0;
    unit->unk_18 = 0;
    unit->unk_1d_0 = 0;
    unit->unk_1b = -1;
    unit->unk_1a = -1;
    unit->unk_1d_6 = 0;
    unit->unk_1d_7 = 0;
    unit->unk_1e_0 = 1;
    unit->unk_1f = 0;
    unit->unk_1e_1 = 0;
    unit->startX = unit->x;
    unit->unk_0c = unit->x;
    unit->startUnk_04 = unit->unk_04;
    unit->startHp = unit->hp;
    cell->state = 2;
    cell->unit = slot;
    return unit;
}
