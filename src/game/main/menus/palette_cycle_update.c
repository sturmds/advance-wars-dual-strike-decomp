#include "types.h"
#include "game/task.h"

void Palette_Load(const void* src, u32 offset, u32 size);
int MainLoop_GetFrameCount(void);

extern const u16 data_02149280[];

/* Update of the task PaletteCycle_Start makes: copies one color from the table
 * data_02149280 (second row of 16) into color 14 of the task's palette
 * slot, stepping to the next color every 4 frames. */
void PaletteCycle_Update(Task* task) {
  int frame = MainLoop_GetFrameCount();

  Palette_Load((const u8*)data_02149280 + 32 + ((frame & 0x3c) >> 1),
               (u16)(task->unk_52 * 32 + 28), 2);
}
