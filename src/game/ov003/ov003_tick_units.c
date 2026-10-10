#include "types.h"
#include "game/ov003_unit.h"

extern Ov003Unit data_ov003_02369538[16];

void func_ov003_02358904(void);
void Ov003_SnapshotUnits(void);

/* Per frame: updates the units (func_ov003_02358904), snapshots them, then
 * counts down unk_15, unk_08 and unk_11 of each live unit that has unk_17_6
 * clear, stopping each at 0. */
void Ov003_TickUnits(void)
{
    Ov003Unit *unit = data_ov003_02369538;
    int i;

    func_ov003_02358904();
    Ov003_SnapshotUnits();
    for (i = 0; i < 16; i++, unit++) {
        if (unit->hp != 0 && !unit->unk_17_6) {
            if (unit->unk_15 != 0) {
                unit->unk_15--;
            }
            /* needed to match: Ov003Unit.unk_08 is s16 in the header, but the
             * original reads it unsigned */
            if (*(u16 *)&unit->unk_08 != 0) {
                (*(u16 *)&unit->unk_08)--;
            }
            if (unit->unk_11 != 0) {
                unit->unk_11--;
            }
        }
    }
}
