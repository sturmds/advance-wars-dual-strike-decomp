#include "types.h"
#include "game/task.h"
#include "game/Unk_022a7b80.h"
#include "game/Unk_02088d04.h"

s32 HwDiv32(s32 numerator, s32 denominator);
s32 HwMod32(s32 numerator, s32 denominator);
void Text_PrintImmediate(int x, int y, u16 *map, u32 textId, u16 unk_30, int arg);
void Layer0_PutRatio(int x, int y, u32 first, u32 second);
void Layer0_PutNumber(int x, int y, u32 value);
void Layer0_PutNumberThenText4(int x, int y, u32 value);
void Gfx_RequestLayerCopy(int layer);
void Font_LoadDigits(int baseTile);
void func_01ff8c84(int layer);
void func_0201e6e8(int layer, int x, int y, int a3);
void func_02026ddc(int x, int y, u32 value);
void func_02026e68(int x, int y, u32 value);
int func_020eaa8c(int mode);
int Survival_GetMapCount(int mode);

extern Unk_022a7b80 data_022a7b80;
extern Unk_027c0280 *data_027c0280;

/* Draws the two summary rows (19 and 21) of layer 0 for the mode variant in
 * data_022a7b80.unk_07: 0/3 a time in seconds (h:mm), 2/5 a number, 1/4 a
 * number with a suffix. Row 19 shows the mode's value from func_020eaa8c.
 * In modes 0-2 it also shows the mode's map count (Survival_GetMapCount), and once
 * unk_06 reaches that count row 21 shows the mode's value minus unk_08;
 * otherwise, and always in modes 3-5, row 21 shows unk_06. Then scrolls
 * layers 0 and 1 up by 64 pixels. */
void Survival_DrawRunSummary(Task *task)
{
    int left;
    u8 layer1;
    u8 layer0;

    func_01ff8c84(0);
    func_01ff8c84(1);
    Font_LoadDigits(0);
    func_0201e6e8(0, 18, 21, 6);
    switch (data_022a7b80.unk_07) {
    case 0:
        func_02026e68(1, 19, Survival_GetMapCount(0));
        Text_PrintImmediate(7, 19, data_027c0280->layer0Map, 0x79000005, 0, 0);
        Layer0_PutRatio(19, 19, HwDiv32(func_020eaa8c(0), 3600), HwMod32(HwDiv32(func_020eaa8c(0), 60), 60));
        if (data_022a7b80.unk_06 == Survival_GetMapCount(0)) {
            left = func_020eaa8c(0) - data_022a7b80.unk_08;
            Text_PrintImmediate(7, 21, data_027c0280->layer0Map, 0x79000006, 0, 0);
            Layer0_PutRatio(19, 21, HwDiv32(left, 3600), HwMod32(HwDiv32(left, 60), 60));
        } else {
            Text_PrintImmediate(7, 21, data_027c0280->layer0Map, 0x7900000e, 0, 0);
            func_02026ddc(19, 21, data_022a7b80.unk_06);
        }
        break;
    case 3:
        Text_PrintImmediate(1, 19, data_027c0280->layer0Map, 0x79000002, 0, 0);
        Text_PrintImmediate(7, 19, data_027c0280->layer0Map, 0x79000005, 0, 0);
        Layer0_PutRatio(19, 19, HwDiv32(func_020eaa8c(3), 3600), HwMod32(HwDiv32(func_020eaa8c(3), 60), 60));
        Text_PrintImmediate(7, 21, data_027c0280->layer0Map, 0x7900000e, 0, 0);
        func_02026ddc(19, 21, data_022a7b80.unk_06);
        break;
    case 2:
        func_02026e68(1, 19, Survival_GetMapCount(2));
        Text_PrintImmediate(7, 19, data_027c0280->layer0Map, 0x79000008, 0, 0);
        Layer0_PutNumber(19, 19, func_020eaa8c(2));
        if (data_022a7b80.unk_06 == Survival_GetMapCount(2)) {
            left = func_020eaa8c(2) - data_022a7b80.unk_08;
            Text_PrintImmediate(7, 21, data_027c0280->layer0Map, 0x79000009, 0, 0);
            Layer0_PutNumber(19, 21, left);
        } else {
            Text_PrintImmediate(7, 21, data_027c0280->layer0Map, 0x7900000e, 0, 0);
            func_02026ddc(19, 21, data_022a7b80.unk_06);
        }
        break;
    case 5:
        Text_PrintImmediate(1, 19, data_027c0280->layer0Map, 0x79000002, 0, 0);
        Text_PrintImmediate(7, 19, data_027c0280->layer0Map, 0x79000008, 0, 0);
        Layer0_PutNumber(19, 19, func_020eaa8c(5));
        Text_PrintImmediate(7, 21, data_027c0280->layer0Map, 0x7900000e, 0, 0);
        func_02026ddc(19, 21, data_022a7b80.unk_06);
        break;
    case 1:
        func_02026e68(1, 19, Survival_GetMapCount(1));
        Text_PrintImmediate(7, 19, data_027c0280->layer0Map, 0x7900000b, 0, 0);
        Layer0_PutNumberThenText4(19, 19, func_020eaa8c(1));
        if (data_022a7b80.unk_06 == Survival_GetMapCount(1)) {
            left = func_020eaa8c(1) - data_022a7b80.unk_08;
            Text_PrintImmediate(7, 21, data_027c0280->layer0Map, 0x7900000c, 0, 0);
            Layer0_PutNumberThenText4(19, 21, left);
        } else {
            Text_PrintImmediate(7, 21, data_027c0280->layer0Map, 0x7900000e, 0, 0);
            func_02026ddc(19, 21, data_022a7b80.unk_06);
        }
        break;
    default:
        Text_PrintImmediate(1, 19, data_027c0280->layer0Map, 0x79000002, 0, 0);
        Text_PrintImmediate(7, 19, data_027c0280->layer0Map, 0x7900000b, 0, 0);
        Layer0_PutNumberThenText4(19, 19, func_020eaa8c(4));
        Text_PrintImmediate(7, 21, data_027c0280->layer0Map, 0x7900000e, 0, 0);
        func_02026ddc(19, 21, data_022a7b80.unk_06);
        break;
    }
    Gfx_RequestLayerCopy(0);
    layer1 = data_027c0280->layer1Bg;
    layer0 = data_027c0280->layer0Bg;
    data_027c0280->bgScrollY[layer1] = -64;
    data_027c0280->bgScrollY[layer0] = data_027c0280->bgScrollY[layer1];
    task->unk_44 = 0;
}
