#include "types.h"
#include "game/task.h"
#include "game/ov005_task.h"

Task* PaletteFade_StartFromBlack(Ov005Task* parent, void* palette, int arg2,
                                 int arg3, int arg4);

extern u8* data_ov005_0236bb90;

/* Starts a palette task on the data at offset 0x8360 of the large work block; its purpose is a guess. */
void Ov005Screen_StartPaletteTask(Ov005Task* task) {
  task->paletteTask =
      PaletteFade_StartFromBlack(task, data_ov005_0236bb90 + 0x8360, 180, 0, 4);
}
