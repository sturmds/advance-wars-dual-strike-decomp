#include "types.h"

void Sprite_QueueGroup(int list, int x, int y, u16 xBits, u16 yBits, const void *group, u16 tileAdd, u16 attr);
extern u8 gCoPortraitSprite[];
#define OBJ_X_FLIP 0x1000

void func_02002598(int x, int y, int xBits, int yBits, int tileAdd, int attr, int list, int mirror)
{
    u16 xAttr = xBits & 0xfe00;
    u16 yAttr = yBits & 0xff00;

    if (mirror) {
        xAttr |= OBJ_X_FLIP;
    }
    Sprite_QueueGroup(list, x, y - 32, xAttr, yAttr, gCoPortraitSprite, tileAdd, attr);
}
