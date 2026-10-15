#include "types.h"
#include "game/Unk_02088d04.h"

void Mem_Copy16(const void* src, void* dst, u32 size);
void Palette_Flush(void);

void func_02003154(const void* src, int offset, u32 size) {
  Mem_Copy16(src, (u16*)data_027c0280->paletteBuffer + (offset >> 1), size);
  Palette_Flush();
}
