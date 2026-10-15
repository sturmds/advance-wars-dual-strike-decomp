#include "types.h"
#include "game/Unk_021694c0.h"
#include "game/math.h"

extern Unk_021694c0* data_021694c0;

int func_020f22c8(void) {
  return HwDiv32(data_021694c0->unk_3c * 100, data_021694c0->unk_38) >= 90;
}
