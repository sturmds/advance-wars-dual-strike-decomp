#include "types.h"
#include "game/play_records.h"
#include "game/Unk_02088d04.h"

extern Unk_027c0280 *data_027c0280;

void Text_PrintImmediate(s16 x, s16 y, u16 *map, u32 textId, u16 unk_30, int arg);
void Layer0_PutNumberThenText4(int x, int y, u32 value);
void func_02026e68(int x, int y, u32 value);
int func_02047944(int mode);
int func_020eaa8c(int mode);
int func_020eaa9c(int mode);

/* Survival mode variant 1: on layer 0 from column x, prints the mode's map
 * count (func_020eaa9c), the labels, the mode's value from func_020eaa8c on
 * row y and, on row y + 2, what the saved record left of that value, or text
 * 0xc00000af when no record is saved. */
void SurvivalSelect_DrawMode1Record(int x, int y)
{
    func_02026e68(x, y, func_020eaa9c(1));
    Text_PrintImmediate(x + 11, y, data_027c0280->layer0Map, 0x7900000b, 0, 0);
    Text_PrintImmediate(x + 11, y + 2, data_027c0280->layer0Map, 0x7900000c, 0, 0);
    Layer0_PutNumberThenText4(x + 23, y, func_020eaa8c(1));
    if (func_02047944(1) == 1) {
        Layer0_PutNumberThenText4(x + 23, y + 2, func_020eaa8c(1) - gPlayRecords.survivalRecords[1].value);
    } else {
        Text_PrintImmediate(x + 22, y + 2, data_027c0280->layer0Map, 0xc00000af, 0, 0);
    }
}
