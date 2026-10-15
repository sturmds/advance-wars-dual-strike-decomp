#include "types.h"
#include "game/Unk_02080440.h"
#include "game/Unk_02088d04.h"

void Mem_CopyAuto(const void* src, void* dst, u32 size);
void Gfx_LoadPlayerWindowFrame(int player);

void func_020f8848(void) {
  Mem_CopyAuto(data_027c0280->layer0Map, data_027c0280->unk_e4, BG_SCREEN_SIZE);
  Mem_CopyAuto(data_027c0280->layer1Map, data_027c0280->unk_f8, BG_SCREEN_SIZE);
  Gfx_LoadPlayerWindowFrame(
      data_027c027c->unk_48[data_027c0284]->currentPlayer);
}
