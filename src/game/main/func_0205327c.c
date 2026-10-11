#include "types.h"
#include "game/menu_state.h"
#include "game/screen_other_map.h"

void Mem_Copy32(const void *src, void *dst, u32 size);
void Bg_ClearRect(void *target, int x, int y, int w, int h);
void Gfx_RequestLayerCopy(int layer);

extern MenuState gMenuState;

/* Day banner layer refresh (guess from DayBanner_Init and _CloseUpdate, which call it
 * after setting the scroll): copies the saved banner map from gMenuState back to
 * layer 0, then clears the tile rows above the scroll position and the rows below
 * the 192-pixel screen height, so only the visible part of the banner is left. */
void func_0205327c(void)
{
    int top;
    int bottom;
    u16 row;

    Mem_Copy32(&gMenuState, data_027c0280->layer0Map, 0x800);
    top = (s16)data_027c0280->bgScrollY[data_027c0280->layer0Bg];
    if (top < 0) {
        top = 0;
    }
    Bg_ClearRect(data_027c0280->layer0Map, 0, 0, 32, (u32)(top << 13) >> 16);
    bottom = (s16)(((s16)data_027c0280->bgScrollY[data_027c0280->layer0Bg] + 199) & ~7);
    if (bottom > 256) {
        bottom = 256;
    }
    row = bottom << 13 >> 16;
    Bg_ClearRect(data_027c0280->layer0Map, 0, (u16)row, 32, (u16)(32 - row));
    Gfx_RequestLayerCopy(0);
}
