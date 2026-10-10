#include "types.h"
#include "game/Unk_ov004_022bf158.h"

void Ov004_InitScale(Unk_ov004_022bf158 *state) {
    Unk_ov004_022bf158_818 *scale = &state->unk_818;

    state->unk_818.unk_00 = 0;
    scale->unk_04 = 0;
    scale->unk_08 = 0;
    scale->unk_0a = 0;
    scale->unk_0c = 256;
    scale->unk_0e = 256;
    scale->unk_10 = 0;
}
