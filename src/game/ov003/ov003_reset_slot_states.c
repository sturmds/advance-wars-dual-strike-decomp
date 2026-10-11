#include "types.h"
#include "game/Unk_02160ac0.h"
#include "game/ov003_unit.h"

u16 Wireless_GetChildAids(void);
void func_ov003_0236322c(void);

/* When phase is 1 and every slot in play (the current slot, or each linked
 * player) has state 4, sets those slots to state 1 and calls
 * func_ov003_0236322c, which starts a task. */
void Ov003_ResetSlotStates(void)
{
    int i;
    int active;

    if (data_ov003_02364928->phase != 1) {
        return;
    }
    active = 0;
    if (data_02160ac0->unk_7c == 0) {
        if (data_ov003_02364928->slots[data_ov003_02364928->currentSlot].state != 4) {
            active = 1;
        }
    } else {
        for (i = 0; i < 8; i++) {
            if ((Wireless_GetChildAids() | 1) & (1 << i)) {
                if (data_ov003_02364928->slots[i].state != 4) {
                    active = 1;
                }
            }
        }
    }
    if (active == 1) {
        return;
    }
    for (i = 0; i < 8; i++) {
        if ((Wireless_GetChildAids() | 1) & (1 << i)) {
            data_ov003_02364928->slots[i].state = 1;
        }
    }
    func_ov003_0236322c();
}
