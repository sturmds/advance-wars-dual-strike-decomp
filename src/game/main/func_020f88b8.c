#include "types.h"
#include "game/Unk_02088d04.h"

void Mem_CopyAuto(void*, void*, int, void*);
extern u8 data_02139810[];

void func_020f88b8(void) {
  u8* t0 = data_027c0280->unk_e4;
  Mem_CopyAuto(data_02139810, t0 + 0x3ae0, 160, t0);
}
