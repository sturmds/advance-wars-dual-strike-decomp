#include "types.h"
#include "sdk/gx.h"
#include "game/wallpaper_task.h"
#include "game/Unk_02080440.h"
#include "game/Unk_02088d04.h"

extern Unk_02080440 *data_027c027c;
extern Unk_027c0280 *data_027c0280;
extern Unk_027c0288 data_027c0288;
extern volatile s16 gScreenShakeY;
extern volatile s16 gScreenShakeX;

/* Needed to match: the scroll fields are read through a volatile view so
 * their loads stay in source order. */
#define VGFX ((volatile Unk_027c0280 *)data_027c0280)

/* Packs x and y into one BGxHOFS/BGxVOFS word (9 bits each) and writes it. */
static inline void SetBgOffset(volatile u32 *reg, int x, int y)
{
    *reg = (x & 0x1ff) | ((y << 16) & 0x1ff0000);
}

/* Task update of the single wallpaper: every 2 frames scrolls layer 3 one
 * pixel (wrapping after 320), and writes BG3's scroll, less the screen
 * shake, to whichever engine the current screen uses. */
void WallpaperSingleTask_Update(WallpaperSingleTask *task)
{
    int x;
    int y;

    if ((task->frame & 1) == 0) {
        data_027c0280->bgScrollY[data_027c0280->layer3Bg] = task->scrollY;
        task->scrollY++;
        if (task->scrollY > 320) {
            task->scrollY -= 320;
        }
    }
    if (data_027c027c == data_027c0288.unk_00) {
        y = VGFX->bgScrollY[3] - gScreenShakeY;
        x = VGFX->bgScrollX[3] - gScreenShakeX;
        SetBgOffset(IO_BG3_SCROLL, x, y);
    } else {
        y = VGFX->bgScrollY[3] - gScreenShakeY;
        x = VGFX->bgScrollX[3] - gScreenShakeX;
        SetBgOffset(REG_DB_BG3OFS, x, y);
    }
    task->frame++;
}
