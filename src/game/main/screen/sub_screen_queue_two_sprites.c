#include "types.h"
#include "game/screen_other_map.h"

void Sprite_QueueAt(int list, int x, int y, void *data, int attr, int tile);

extern u8 data_027c0148;
extern u8 data_027c0030;

/* Queues two sprites on the sub screen at x offset +160 and +176 (wrapped to 9 bits)
 * and y 104 and 120; which sprites they are is not known yet. */
void SubScreen_QueueTwoSprites(int x)
{
    Unk_027c0280 *saved0280;
    Unk_02080440 *saved027c;

    saved0280 = data_027c0280;
    Screen_SetCurrentGfx(data_027c0288.unk_04);
    saved027c = data_027c027c;
    Screen_SetCurrent(data_027c0288.unk_04);
    Sprite_QueueAt(2, (x + 160) & 0x1ff, 104, &data_027c0148, 0x5060, 0);
    Sprite_QueueAt(4, (x + 176) & 0x1ff, 120, &data_027c0030, 0x6078, 0);
    Screen_SetCurrent(saved027c);
    Screen_SetCurrentGfx(saved0280);
}
