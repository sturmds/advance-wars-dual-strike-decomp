#include "types.h"
#include "game/Unk_ov004_022bf158.h"

void func_ov004_022bb168(Unk_ov004_022bf158 *state, int slot);

void Ov004_DestroyAllSlotTasks(Unk_ov004_022bf158 *state) {
    int slot;

    for (slot = 0; slot < OV004_SLOT_COUNT; slot++) {
        func_ov004_022bb168(state, slot);
    }
}
