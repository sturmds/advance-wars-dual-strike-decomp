#include "types.h"
#include "game/ov003_unit.h"

extern u16 data_ov003_02364cdc[][2];
static u8 data_ov003_02364b14[16] = {
#include "data/ov003/data_ov003_02364b14.inc.c"
};
static u8 data_ov003_02364b24[32] = {
#include "data/ov003/data_ov003_02364b24.inc.c"
};
extern u8 data_027c0028;

s32 HwDiv32(s32 numerator, s32 denominator);
s32 HwMod32(s32 numerator, s32 denominator);
int MainLoop_GetFrameCount(void);
void Sprite_Queue(int list, int x, int y, void* data, int attr, int tile);
int Ov003_IsRowPowerActive(int rowNumber);

#define STATE data_ov003_02364928
#define ROW (STATE->rows[row - 1])
#define CELL_X(i) (((column + 1) * 8 + (i) * 6) & 0x1ff)

/* Draws the charge (Ov003Row.charge) of a row (1 to 5) as a bar of sprites at
 * y 184, from tile column unk_3b_4 * 4 + 1, 6 pixels apart. The bar has one
 * sprite for each 200 of the charge limit (rounded up); a sprite shows 4 steps
 * of fill. When the bar is full its sprites bob in a wave over time. While
 * Ov003_IsRowPowerActive returns 1 for the row, it draws one other sprite at y 176. */
void Ov003Row_DrawCharge(int row) {
  int column;
  int cells;
  int top;
  int fill;
  int i;

  column = ROW.unk_3b_4 * 4;
  if (Ov003_IsRowPowerActive(row) == 1) {
    Sprite_Queue(1, ((column + 1) * 8) & 0x1ff, 176, data_ov003_02364b14, 24,
                 0);
    return;
  }
  cells = HwDiv32(data_ov003_02364cdc[ROW.co][0], 200);
  if (HwMod32(data_ov003_02364cdc[ROW.co][0], 200) != 0) {
    cells++;
  }
  top = cells * 4 - 1;
  fill = HwDiv32(top * ROW.charge, data_ov003_02364cdc[ROW.co][0]);
  for (i = 0; i < cells; i++) {
    if (fill == top) {
      Sprite_Queue(
          1, CELL_X(i),
          (184 - data_ov003_02364b24[(MainLoop_GetFrameCount() - i) & 31]) &
              0xff,
          &data_027c0028, 22, 0);
    } else if (fill >= i * 4 + 3) {
      Sprite_Queue(1, CELL_X(i), 184, &data_027c0028, 22, 0);
    } else if (fill > i * 4) {
      Sprite_Queue(1, CELL_X(i), 184, &data_027c0028, (fill - i * 4) * 2 + 16,
                   0);
    } else {
      Sprite_Queue(1, CELL_X(i), 184, &data_027c0028, 16, 0);
    }
  }
}
