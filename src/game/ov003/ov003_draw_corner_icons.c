#include "types.h"
#include "game/ov003_unit.h"
#include "game/oam_cells.h"

int func_ov003_02357308(void);
int func_ov003_023572b0(void);
void Oam_PutCells(int attr1, int attr0, const void* cells, int attr2, int tile);

/* High byte of Ov003State.unk_1d6. */
#define STATE_BYTE_1D7 (((u8*)&data_ov003_02364928->unk_1d6)[1])

/* Draws a sprite at the bottom right corner (224, 160) for each of the two
 * conditions. The byte at 0x1d7 of the state is cleared meanwhile and restored. */
void Ov003_DrawCornerIcons(void) {
  u8 saved = STATE_BYTE_1D7;

  STATE_BYTE_1D7 = 0;
  if (func_ov003_02357308() == 1) {
    Oam_PutCells(224, 160, &data_027c0048, 0, 0x518);
  }
  if (func_ov003_023572b0() == 1) {
    Oam_PutCells(224, 160, &data_027c0048, 0, 0x528);
  }
  STATE_BYTE_1D7 = saved;
}
