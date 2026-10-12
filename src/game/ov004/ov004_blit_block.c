#include "types.h"
#include "game/Unk_ov004_022bf158.h"

void Ov004_BlitBlockWithFlags(Unk_ov004_022bf158* state, int x, int y,
                              const u16* src, u16 flags);

void Ov004_BlitBlock(Unk_ov004_022bf158* state, int x, int y, const u16* src) {
  Ov004_BlitBlockWithFlags(state, x, y, src, 0);
}
