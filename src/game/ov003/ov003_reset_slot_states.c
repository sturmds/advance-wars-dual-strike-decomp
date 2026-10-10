#include "types.h"
#include "game/Unk_02160ac0.h"
#include "game/ov003_unit.h"

u16 func_0207a1dc(void);
void func_ov003_0236322c(void);

/* When unk_1da_0 is 1 and every slot in play (the current slot, or each linked
 * player) has state 4, sets those slots to state 1 and calls
 * func_ov003_0236322c, which starts a task. */
void Ov003_ResetSlotStates(void)
{
    int i;
    int active;

    if (data_ov003_02364928->unk_1da_0 != 1) {
        return;
    }
    active = 0;
    if (data_02160ac0->unk_7c == 0) {
        if (data_ov003_02364928->slots[data_ov003_02364928->currentSlot].unk_01_4 != 4) {
            active = 1;
        }
    } else {
        for (i = 0; i < 8; i++) {
            if ((func_0207a1dc() | 1) & (1 << i)) {
                if (data_ov003_02364928->slots[i].unk_01_4 != 4) {
                    active = 1;
                }
            }
        }
    }
    if (active == 1) {
        return;
    }
    for (i = 0; i < 8; i++) {
        if ((func_0207a1dc() | 1) & (1 << i)) {
            data_ov003_02364928->slots[i].unk_01_4 = 1;
        }
    }
    func_ov003_0236322c();
}
