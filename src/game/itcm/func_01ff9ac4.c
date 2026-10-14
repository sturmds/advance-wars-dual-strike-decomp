#include "types.h"
#include "game/Unk_02088d04.h"

void Mem_CopyAuto(void*, void*, int);
extern u8 data_02139fd0[];
extern Unk_027c0280* data_027c0280;

void func_01ff9ac4(void) {
  Mem_CopyAuto(data_02139fd0, data_027c0280->objVram + 0x6c00, 0x800);
}
