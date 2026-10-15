#include "types.h"
#include "game/Unk_02088d04.h"

extern u8* data_02297790;

void* func_0208c138(void) {
  data_02297790 = data_027c0280->bgVram;
  return &data_02297790;
}
