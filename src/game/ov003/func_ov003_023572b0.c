#include "types.h"
#include "game/ov003_unit.h"

int func_ov003_023621b0(void);

int func_ov003_023572b0(void)
{
    if (func_ov003_023621b0() == 0) {
        return 0;
    }
    return data_ov003_02364928->slots[data_ov003_02364928->currentSlot].unk_01_4 == 2;
}
