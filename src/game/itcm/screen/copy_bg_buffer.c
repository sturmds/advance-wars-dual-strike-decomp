#include "types.h"
#include "game/screen_other_map.h"

void func_01ff9ac4(void);

/* Runs func_01ff9ac4, which copies 0x800 bytes into the current screen's graphics
 * buffer, for the screen at data_027c0288.unk_04, then restores the current one. */
void Gfx_CopyBgBufferOnScreen1(void)
{
    Unk_027c0280 *saved0280 = data_027c0280;

    Screen_SetCurrentGfx(data_027c0288.unk_04);
    func_01ff9ac4();
    Screen_SetCurrentGfx(saved0280);
}
