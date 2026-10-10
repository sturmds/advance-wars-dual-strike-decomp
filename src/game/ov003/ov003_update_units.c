#include "types.h"
#include "game/ov003_unit.h"

extern Ov003Unit data_ov003_02369538[16];
extern u8 data_ov003_02365d10[];       /* per tile code: bits 5-7 row, bits 0-4 terrain */

int func_ov003_02362258(void);
void Ov003Unit_Draw(Ov003Unit *unit);

#define STATE data_ov003_02364928
#define MAP data_ov003_02364a88

/* Calls Ov003Unit_Draw for each unit that is in use, unless one of these
 * holds: unk_17_6 is set; the state is in mode 1 and the unit's row is neither
 * the current slot's row nor Ov003State.row; the unit stands on terrain 4 whose
 * mark is 0; unk_08 is set while unk_1c8 has bit 1 and func_ov003_02362258
 * returns 1. */
void Ov003_UpdateUnits(void)
{
    Ov003Unit *unit = data_ov003_02369538;
    int i;

    for (i = 0; i < 16; i++, unit++) {
        int tile;

        if (unit->hp == 0) {
            continue;
        }
        if (unit->unk_17_6) {
            continue;
        }
        if (STATE->unk_1da_0 == 1
            && unit->unk_12_3 != STATE->slots[STATE->currentSlot].row
            && unit->unk_12_3 != STATE->row) {
            continue;
        }
        tile = (unit->y >> 7) * MAP->width + (unit->x >> 7);
        if ((data_ov003_02365d10[MAP->cells[tile]] & 31) == 4 && MAP->marks[tile] == 0) {
            continue;
        }
        if ((u16)unit->unk_08 != 0 && (STATE->frameCount & 2) && func_ov003_02362258() == 1) {
            continue;
        }
        Ov003Unit_Draw(unit);
    }
}
