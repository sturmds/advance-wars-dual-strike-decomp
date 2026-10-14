#include "types.h"
#include "game/Unk_02088d04.h"
#include "game/task.h"

void Palette_Load(const void* src, u32 offset, u32 size);
int MainLoop_GetFrameCount(void);
extern const u16 data_02149280[];

void func_02082ac8(Task* task) {
  int frame = MainLoop_GetFrameCount();

  Palette_Load(
      (const u8*)data_02149280 + PALETTE_16_SIZE + ((frame & 0x3c) >> 1),
      (u16)(task->unk_52 * PALETTE_16_SIZE + 28), 2);
}
