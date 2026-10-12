#include "types.h"
#include "game/ov005_task.h"
#include "game/Unk_02080440.h"
#include "game/Unk_02088d04.h"
#include "game/Unk_ov005_0236bba8.h"

void Screen_SetCurrent(void* state);
void Screen_SetCurrentGfx(void* state);
void Gfx3d_InitScene(void);
int func_02028644(void);
void Gx_DrawTexturedQuadFormat(int x, int y, int a2, int a3, int a4, int a5,
                               int a6, int a7, int a8, int a9);
void Sprite_QueueCoSlotFrame(int a0, u16 x, u16 y, int a3);

extern Unk_02080440* data_027c027c;
extern Unk_027c0280* data_027c0280;
extern Unk_027c0288 data_027c0288;

/* On the first screen, inside a 3D scene: for each filled cell of the CO
 * grid, two Gx_DrawTexturedQuadFormat draws (at x and x + 32) and one queued sprite.
 * The n-th filled cell passes n-based offsets, so each gets its own
 * graphics slot. */
void Ov005_DrawCoGridCells(Ov005Task* task) {
  Unk_027c0280* saved0280;
  int i;
  Unk_ov005_0236bba8* work;
  int drawn;
  Ov005CoGridEntry* cell;
  Unk_02080440* saved027c;

  work = task->work;
  saved0280 = data_027c0280;
  Screen_SetCurrentGfx(data_027c0288.unk_00);
  saved027c = data_027c027c;
  Screen_SetCurrent(data_027c0288.unk_00);
  Gfx3d_InitScene();
  drawn = 0;
  cell = work->grid;
  for (i = 0; i < OV005_CO_GRID_ENTRIES; cell++, i++) {
    if (cell->coId != -1) {
      Gx_DrawTexturedQuadFormat(cell->x, cell->y, 0, 2, 1, 3, drawn * 384,
                                drawn * 32, 0, 0);
      Gx_DrawTexturedQuadFormat(cell->x + 32, cell->y, 0, 1, 1, 3,
                                (drawn * 12 + 8) * 32, drawn * 32, 0, 0);
      Sprite_QueueCoSlotFrame(0, cell->x & 0x1ff, cell->y & 0xff, 0);
      drawn++;
    }
  }
  func_02028644();
  Screen_SetCurrent(saved027c);
  Screen_SetCurrentGfx(saved0280);
}
