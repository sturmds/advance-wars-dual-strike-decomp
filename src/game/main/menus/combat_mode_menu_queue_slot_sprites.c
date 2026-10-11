#include "types.h"

int HwMod32(int a, int b);
void func_02001fa8(int x, int y, int attr, int list);
void func_0202b654(int a0, u16 x, u16 y, int a3);

/* Queues the sprites of the seven slot buttons of the Combat mode menu: a
 * frame and a picture per slot, 3 slots per row. */
void CombatModeMenu_QueueSlotSprites(void)
{
    int x = 48;
    int y = 64;
    int i;

    for (i = 0; i < 7; i++) {
        func_0202b654(0, x & 0x1ff, y & 0xff, 0);
        func_02001fa8((x + 24) & 0x1ff, (y + 16) & 0xff, i * 12 | i << 12, 1);
        x += 56;
        if (HwMod32(i, 3) == 2) {
            x -= 168;
            y += 24;
        }
    }
}
