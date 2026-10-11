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
void Gfx_ClearLayerMap(int layer);
void func_0201e6e8(int layer, int x, int y, int a3);
void func_02026ddc(int x, int y, u32 value);
void Survival_DrawMapCount(int x, int y, u32 value);
int Survival_GetModeBudget(int mode);
int Survival_GetMapCount(int mode);

extern Unk_022a7b80 data_022a7b80;
extern Unk_027c0280 *data_027c0280;

/* Draws the two summary rows (19 and 21) of layer 0 for the mode variant in
 * data_022a7b80.mode: 0/3 a time in seconds (h:mm), 2/5 a number, 1/4 a
 * number with a suffix. Row 19 shows the mode's value from Survival_GetModeBudget.
 * In modes 0-2 it also shows the mode's map count (Survival_GetMapCount), and once
 * mapIndex reaches that count row 21 shows the mode's value minus budgetLeft;
 * otherwise, and always in modes 3-5, row 21 shows mapIndex. Then scrolls
 * layers 0 and 1 up by 64 pixels. */
void Survival_DrawRunSummary(Task *task)
{
    int left;
    u8 layer1;
    u8 layer0;

    Gfx_ClearLayerMap(0);
    Gfx_ClearLayerMap(1);
    Font_LoadDigits(0);
    func_0201e6e8(0, 18, 21, 6);
    switch (data_022a7b80.mode) {
    case 0:
        Survival_DrawMapCount(1, 19, Survival_GetMapCount(0));
        Text_PrintImmediate(7, 19, data_027c0280->layer0Map, 0x79000005, 0, 0);
        Layer0_PutRatio(19, 19, HwDiv32(Survival_GetModeBudget(0), 3600), HwMod32(HwDiv32(Survival_GetModeBudget(0), 60), 60));
        if (data_022a7b80.mapIndex == Survival_GetMapCount(0)) {
            left = Survival_GetModeBudget(0) - data_022a7b80.budgetLeft;
            Text_PrintImmediate(7, 21, data_027c0280->layer0Map, 0x79000006, 0, 0);
            Layer0_PutRatio(19, 21, HwDiv32(left, 3600), HwMod32(HwDiv32(left, 60), 60));
        } else {
            Text_PrintImmediate(7, 21, data_027c0280->layer0Map, 0x7900000e, 0, 0);
            func_02026ddc(19, 21, data_022a7b80.mapIndex);
        }
        break;
    case 3:
        Text_PrintImmediate(1, 19, data_027c0280->layer0Map, 0x79000002, 0, 0);
        Text_PrintImmediate(7, 19, data_027c0280->layer0Map, 0x79000005, 0, 0);
        Layer0_PutRatio(19, 19, HwDiv32(Survival_GetModeBudget(3), 3600), HwMod32(HwDiv32(Survival_GetModeBudget(3), 60), 60));
        Text_PrintImmediate(7, 21, data_027c0280->layer0Map, 0x7900000e, 0, 0);
        func_02026ddc(19, 21, data_022a7b80.mapIndex);
        break;
    case 2:
        Survival_DrawMapCount(1, 19, Survival_GetMapCount(2));
        Text_PrintImmediate(7, 19, data_027c0280->layer0Map, 0x79000008, 0, 0);
        Layer0_PutNumber(19, 19, Survival_GetModeBudget(2));
        if (data_022a7b80.mapIndex == Survival_GetMapCount(2)) {
            left = Survival_GetModeBudget(2) - data_022a7b80.budgetLeft;
            Text_PrintImmediate(7, 21, data_027c0280->layer0Map, 0x79000009, 0, 0);
            Layer0_PutNumber(19, 21, left);
        } else {
            Text_PrintImmediate(7, 21, data_027c0280->layer0Map, 0x7900000e, 0, 0);
            func_02026ddc(19, 21, data_022a7b80.mapIndex);
        }
        break;
    case 5:
        Text_PrintImmediate(1, 19, data_027c0280->layer0Map, 0x79000002, 0, 0);
        Text_PrintImmediate(7, 19, data_027c0280->layer0Map, 0x79000008, 0, 0);
        Layer0_PutNumber(19, 19, Survival_GetModeBudget(5));
        Text_PrintImmediate(7, 21, data_027c0280->layer0Map, 0x7900000e, 0, 0);
        func_02026ddc(19, 21, data_022a7b80.mapIndex);
        break;
    case 1:
        Survival_DrawMapCount(1, 19, Survival_GetMapCount(1));
        Text_PrintImmediate(7, 19, data_027c0280->layer0Map, 0x7900000b, 0, 0);
        Layer0_PutNumberThenText4(19, 19, Survival_GetModeBudget(1));
        if (data_022a7b80.mapIndex == Survival_GetMapCount(1)) {
            left = Survival_GetModeBudget(1) - data_022a7b80.budgetLeft;
            Text_PrintImmediate(7, 21, data_027c0280->layer0Map, 0x7900000c, 0, 0);
            Layer0_PutNumberThenText4(19, 21, left);
        } else {
            Text_PrintImmediate(7, 21, data_027c0280->layer0Map, 0x7900000e, 0, 0);
            func_02026ddc(19, 21, data_022a7b80.mapIndex);
        }
        break;
    default:
        Text_PrintImmediate(1, 19, data_027c0280->layer0Map, 0x79000002, 0, 0);
        Text_PrintImmediate(7, 19, data_027c0280->layer0Map, 0x7900000b, 0, 0);
        Layer0_PutNumberThenText4(19, 19, Survival_GetModeBudget(4));
        Text_PrintImmediate(7, 21, data_027c0280->layer0Map, 0x7900000e, 0, 0);
        func_02026ddc(19, 21, data_022a7b80.mapIndex);
        break;
    }
    Gfx_RequestLayerCopy(0);
    layer1 = data_027c0280->layer1Bg;
    layer0 = data_027c0280->layer0Bg;
    data_027c0280->bgScrollY[layer1] = -64;
    data_027c0280->bgScrollY[layer0] = data_027c0280->bgScrollY[layer1];
    task->unk_44 = 0;
}
