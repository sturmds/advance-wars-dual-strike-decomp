#include "types.h"
#include "game/task.h"

extern u16 data_0214a528[16];

void Palette_LoadWithCopy(const void* src, int offset, u32 size);

/* Advances the frame counter unk_64 and loads colour (unk_64 >> 2) & 15 of
 * data_0214a528 into palette byte offset 8. The counter is read as signed to match. */
void PaletteAnimTask_LoadFrame(Task* task) {
  task->unk_64 = task->unk_64 + 1;
  Palette_LoadWithCopy(&data_0214a528[(task->unk_64 >> 2) & 0xf], 8, 2);
}
