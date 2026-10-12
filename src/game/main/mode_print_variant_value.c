#include "types.h"
#include "game/Unk_022a7b80.h"
#include "game/Unk_02088d04.h"

extern Unk_022a7b80 data_022a7b80;
extern Unk_027c0280* data_027c0280;

s32 HwDiv32(s32 numerator, s32 denominator);
s32 HwMod32(s32 numerator, s32 denominator);
void Text_PrintImmediate(int x, int y, u16* map, u32 textId, u16 unk_30,
                         int arg);
void Layer0_PutRatio(int x, int y, u32 first, u32 second);
void Layer0_PutNumber(int x, int y, u32 value);
void Layer0_PutNumberThenText4(int x, int y, u32 value);
void func_02026ddc(int x, int y, u32 value);
void Gfx_RequestLayerCopy(int layer);

/* Prints a label and the value of data_022a7b80 that goes with the mode
 * variant in mode on row 9 of layer 0. */
void Mode_PrintVariantValue(void* task) {
  switch (data_022a7b80.mode) {
    case 0:
      Text_PrintImmediate(7, 9, data_027c0280->layer0Map, 0x79000007, 0, 0);
      Layer0_PutRatio(24, 9, HwDiv32(data_022a7b80.budgetLeft, 3600),
                      HwMod32(HwDiv32(data_022a7b80.budgetLeft, 60), 60));
      break;
    case 2:
      Text_PrintImmediate(7, 9, data_027c0280->layer0Map, 0x7900000a, 0, 0);
      Layer0_PutNumber(24, 9, data_022a7b80.budgetLeft);
      break;
    case 1:
      Text_PrintImmediate(7, 9, data_027c0280->layer0Map, 0x7900000d, 0, 0);
      Layer0_PutNumberThenText4(24, 9, data_022a7b80.budgetLeft);
      break;
    default:
      Text_PrintImmediate(7, 9, data_027c0280->layer0Map, 0x7900000e, 0, 0);
      func_02026ddc(24, 9, data_022a7b80.mapIndex);
      break;
  }
  Gfx_RequestLayerCopy(0);
}
