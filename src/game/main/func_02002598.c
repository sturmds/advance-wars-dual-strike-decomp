#include "types.h"

void func_02021520(int list, int x, int y, u16 xBits, u16 yBits, const void *group, u16 tileAdd, u16 attr);
extern u8 data_02151dac[];
#define OBJ_X_FLIP 0x1000

void func_02002598(int x, int y, int xBits, int yBits, int tileAdd, int attr, int list, int mirror)
{
    u16 xAttr = xBits & 0xfe00;
    u16 yAttr = yBits & 0xff00;

    if (mirror) {
        xAttr |= OBJ_X_FLIP;
    }
    func_02021520(list, x, y - 32, xAttr, yAttr, data_02151dac, tileAdd, attr);
}
