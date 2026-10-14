#include "types.h"
#include "game/task.h"
#include "game/ov005_task.h"
#include "game/ov005_large_work.h"

Task* PaletteFade_StartFromBlack(Ov005Task* parent, void* palette, int arg2,
                                 int arg3, int arg4);

/* Starts a palette task on the large work block's palettes; its purpose is a guess. */
void Ov005Screen_StartPaletteTask(Ov005Task* task) {
  task->paletteTask = PaletteFade_StartFromBlack(
      task, data_ov005_0236bb90->palettes, 180, 0, 4);
}
