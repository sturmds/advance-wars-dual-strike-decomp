#include "types.h"
#include "game/task.h"

void Palette_Load(const void* src, u32 offset, u32 size);

extern const u16 data_02149280[];
extern const TaskScriptCmd data_02159944[];

void PaletteCycle_Start(int palette) {
  Task* task;

  /* Palette index to byte offset, wrapped to 11 bits as the original does. */
  Palette_Load(data_02149280, ((u32)palette << 21) >> 16, 32);
  task = Task_Create(data_02159944, TASK_LIST(3));
  task->unk_52 = palette;
}
