#include "types.h"
#include "game/Unk_02088d04.h"

extern Unk_027c0280* data_027c0280;
void File_UnpackTo(void*, void*, void*);
extern u8 data_02166e08[];

void func_020d2ed0(int a0) {
  u8* t0 = data_027c0280->objVram;
  File_UnpackTo(t0 + (a0 << 5), data_02166e08, t0);
}
