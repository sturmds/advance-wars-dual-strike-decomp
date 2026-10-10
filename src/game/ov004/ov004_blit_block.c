#include "types.h"
#include "game/Unk_ov004_022bf158.h"

void func_ov004_022badec(Unk_ov004_022bf158 *state, int x, int y, const u16 *src, u16 flags);

void Ov004_BlitBlock(Unk_ov004_022bf158 *state, int x, int y, const u16 *src) {
    func_ov004_022badec(state, x, y, src, 0);
}
