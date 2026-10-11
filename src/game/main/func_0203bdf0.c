#include "types.h"

void func_02002a64(int x, int y, int attr, int tile, int list);
BOOL func_0203bf2c(int x, int width);

/* Queues the sprite of CO slot `slot` (two per player) at (x, y) from
 * tile 270, 12 tiles per slot; skipped when off screen. Unknown whether it
 * shows the CO name or a picture, so the name stays an address. */
void func_0203bdf0(int x, int y, int slot)
{
    if (!func_0203bf2c(x, 48)) {
        return;
    }
    func_02002a64(x & 0x1ff, y & 0xff, 0xa000, 270 + slot * 12, 0);
}
