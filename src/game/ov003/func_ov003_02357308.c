#include "types.h"
#include "game/ov003_unit.h"

int Ov003_IsPhase0Or1(void);
int func_ov003_0235d940(int arg);
extern Ov003Unit *data_ov003_02369534;

/* TRUE while the slot kind is 1 and the unit data_ov003_02369534 points to has
 * neither bit 6 nor bit 7 of byte 0x17 set. Used by Ov003_DrawCornerIcons
 * to pick the first sprite. */
int func_ov003_02357308(void)
{
    if (Ov003_IsPhase0Or1() == 0) {
        return 0;
    }
    if (data_ov003_02364928->slots[data_ov003_02364928->currentSlot].unk_01_4 != 1) {
        return 0;
    }
    if (func_ov003_0235d940(data_ov003_02369534->unk_12_3) == 0) {
        return 0;
    }
    if (data_ov003_02369534->unk_17_6 || data_ov003_02369534->unk_17_7) {
        return 0;
    }
    return 1;
}
