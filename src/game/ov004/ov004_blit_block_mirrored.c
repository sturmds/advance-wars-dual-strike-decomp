#include "types.h"
#include "game/Unk_ov004_022bf158.h"

void func_ov004_022badec(Unk_ov004_022bf158 *state, int x, int y, const u16 *src, u16 flags);

void Ov004_BlitBlockMirrored(Unk_ov004_022bf158 *state, int x, int y, const u16 *src) {
    /* 0x400 is the tile horizontal-flip bit */
    func_ov004_022badec(state, x, y, src, 0x400);
}
