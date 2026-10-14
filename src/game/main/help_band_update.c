#include "types.h"
#include "game/Unk_02080440.h"
#include "game/Unk_02088d04.h"
#include "game/task.h"
#include "game/oam_cells.h"

void Screen_SetCurrent(void* state);
void Screen_SetCurrentGfx(void* state);
void Oam_PutCells(int attr1, int attr0, const void* cells, int attr2, int tile);

extern Unk_02080440* data_027c027c;
extern Unk_027c0280* data_027c0280;
extern Unk_027c0288 data_027c0288;

/* Task update of HelpBand_Start. Each frame it draws 16 sprite cells 16 pixels
 * apart across the lower screen, using the tile in unk_52, and scrolls layer 0
 * one pixel when unk_64 is set. */
void HelpBand_Update(Task* task) {
  u32 i;
  Unk_027c0280* saved0280 = data_027c0280;
  Unk_02080440* saved027c;

  Screen_SetCurrentGfx(data_027c0288.unk_04);
  saved027c = data_027c027c;
  Screen_SetCurrent(data_027c0288.unk_04);
  for (i = 0; i < SCREEN_WIDTH / 16; i++) {
    Oam_PutCells((i * 16) & 0x1ff, 0x8b0, &data_027c0060, 0, task->unk_52);
  }
  if (task->unk_64 != 0) {
    data_027c0280->bgScrollX[data_027c0280->layer0Bg]++;
  }
  Screen_SetCurrent(saved027c);
  Screen_SetCurrentGfx(saved0280);
}
