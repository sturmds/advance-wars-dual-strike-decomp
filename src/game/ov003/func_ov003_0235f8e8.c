#include "types.h"
#include "game/ov003_unit.h"

void func_ov003_0235fb38(Ov003Unit *unit);
void Ov003Unit_FireAtLockedTarget(Ov003Unit *unit);
void Ov003Unit_FireForwardOnce(Ov003Unit *unit);
void func_ov003_0235fce8(Ov003Unit *unit);
#define STATE data_ov003_02364928

void func_ov003_0235f8e8(Ov003Unit *unit)
{
    if (unit->unk_15 != 0) {
        return;
    }
    if (unit->unk_1b != -1 && STATE->unk_1df_2 >= 2) {
        func_ov003_0235fb38(unit);
        return;
    }
    if (unit->unk_1c_0) {
        Ov003Unit_FireAtLockedTarget(unit);
        return;
    }
    if (unit->unk_1c_1 && unit->unk_1d_7) {
        Ov003Unit_FireForwardOnce(unit);
        return;
    }
    if (unit->unk_19 != 0) {
        return;
    }
    func_ov003_0235fce8(unit);
}
