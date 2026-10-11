#include "types.h"

void Sprite_Queue(int list, int x, int y, void *data, int attr, int tile);
extern u8 gCoPortraitSprite[];
#define OBJ_X_FLIP 0x1000

void func_0200270c(int x, int y, int attr, int list, int mirror)
{
    u16 xAttr = x & 0xfe00;
    u16 yAttr = y & 0xff00;

    x &= 0x1ff;
    y = (y - 32) & 0xff;
    if (mirror) {
        x |= OBJ_X_FLIP;
    }
    Sprite_Queue(list, x | xAttr, y | yAttr, gCoPortraitSprite, attr, 0);
}
