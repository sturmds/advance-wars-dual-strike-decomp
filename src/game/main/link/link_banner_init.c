#include "types.h"
#include "game/screen_other_map.h"
#include "game/link_banner_task.h"

void Gfx_ClearLayerMap(int layer);
void Gfx_RequestLayerCopy(int layer);
void Bg_ClearRect(u16 *map, int x, int y, int width, u16 height);
const void *Text_Get(u32 textId);
int Text_GetPrintWidth(const void *text);

/* Clears layer 0 (all rows but the help text) and layer 1 of the lower screen,
 * then sets the banner width in tiles from its text: the text width in
 * whole tiles, rounded up to even, plus 2. */
void LinkBanner_Init(LinkBannerTask *task)
{
    Unk_027c0280 *saved0280 = data_027c0280;
    Unk_02080440 *saved027c;

    Screen_SetCurrentGfx(data_027c0288.unk_04);
    saved027c = data_027c027c;
    Screen_SetCurrent(data_027c0288.unk_04);
    Bg_ClearRect(data_027c0280->layer0Map, 0, 0, 32, 22);
    Gfx_ClearLayerMap(1);
    Gfx_RequestLayerCopy(0);
    Gfx_RequestLayerCopy(1);
    Screen_SetCurrent(saved027c);
    Screen_SetCurrentGfx(saved0280);
    task->timer = 0;
    task->width = (((Text_GetPrintWidth(Text_Get(task->textId)) >> 3) + 1) & ~1) + 2;
}
