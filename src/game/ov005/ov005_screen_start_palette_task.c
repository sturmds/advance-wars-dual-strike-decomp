#include "types.h"
#include "game/task.h"
#include "game/ov005_task.h"
#include "game/ov005_large_work.h"
#include "game/palette_fade.h"

/* Starts a palette task on the large work block's palettes; its purpose is a guess. */
void Ov005Screen_StartPaletteTask(Ov005Task* task) {
  task->paletteTask = PaletteFade_StartFromBlack(
      task, data_ov005_0236bb90->palettes, 180, 0, 4);
}
