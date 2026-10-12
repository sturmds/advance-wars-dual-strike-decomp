#include "types.h"
#include "game/ov005_task.h"
#include "game/Unk_ov005_0236bba8.h"

void Ov005_InitCoPaletteButtons(Ov005Task* task) {
  Unk_ov005_0236bba8* work = task->work;
  int i;
  CoPaletteButton* button = work->paletteButtons;

  for (i = 0; i < CO_PALETTE_BUTTONS; i++, button++) {
    button->buttonId = 0xe0 + i;
    work->extraX[i] = button->x + 13;
    work->extraY[i] = button->y + 12;
  }
}
