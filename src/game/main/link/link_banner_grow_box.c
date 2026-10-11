#include "types.h"
#include "game/screen_other_map.h"
#include "game/link_banner_task.h"
#include "game/text_print.h"

void Gfx_RequestLayerCopy(int layer);
void func_0201e6e8(s16 x, s16 y, s16 width, s16 height);
void Task_ResumeScript(LinkBannerTask *task);

/* Task update on the lower screen: draws the layer 1 box in rows 14-17 one
 * tile wider on each side per frame. Once it reaches the banner width, prints
 * the banner text on layer 0 and resumes the script. */
void LinkBanner_GrowBox(LinkBannerTask *task)
{
    Unk_027c0280 *saved0280;
    Unk_02080440 *saved027c;

    task->timer++;
    saved0280 = data_027c0280;
    Screen_SetCurrentGfx(data_027c0288.unk_04);
    saved027c = data_027c027c;
    Screen_SetCurrent(data_027c0288.unk_04);
    func_0201e6e8(16 - task->timer, 14, task->timer * 2, 4);
    Screen_SetCurrent(saved027c);
    Screen_SetCurrentGfx(saved0280);
    /* needed to match: the original compares unsigned */
    if ((u32)(task->timer * 2) < (u32)task->width) {
        return;
    }
    saved0280 = data_027c0280;
    Screen_SetCurrentGfx(data_027c0288.unk_04);
    saved027c = data_027c027c;
    Screen_SetCurrent(data_027c0288.unk_04);
    Text_PrintId(17 - task->timer, 15, data_027c0280->layer0Map, task->textId, 0, 0x100);
    Gfx_RequestLayerCopy(0);
    Screen_SetCurrent(saved027c);
    Screen_SetCurrentGfx(saved0280);
    Task_ResumeScript(task);
}
