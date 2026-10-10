#include "types.h"
#include "game/Unk_ov004_022bf158.h"

extern Unk_ov004_022bf158 *data_ov004_022bf158;

void func_ov004_022bba30(void);

u32 func_ov004_022ade44(void) {
    Unk_ov004_022bf158 *state = data_ov004_022bf158;

    func_ov004_022bba30();
    return state->unk_000;
}
