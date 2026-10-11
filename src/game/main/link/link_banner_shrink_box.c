#include "types.h"
#include "game/screen_other_map.h"
#include "game/link_banner_task.h"

void func_01ff8c84(int layer);
void Gfx_RequestLayerCopy(int layer);
void func_0201e6e8(s16 x, s16 y, s16 width, s16 height);
void Task_ResumeScript(LinkBannerTask *task);

/* Task update on the lower screen: redraws the layer 1 box in rows 14-17 one
 * tile narrower on each side per frame, and resumes the script once it is gone. */
void LinkBanner_ShrinkBox(LinkBannerTask *task)
{
    Unk_027c0280 *saved0280;
    Unk_02080440 *saved027c;

    task->timer--;
    saved0280 = data_027c0280;
    Screen_SetCurrentGfx(data_027c0288.unk_04);
    saved027c = data_027c027c;
    Screen_SetCurrent(data_027c0288.unk_04);
    func_01ff8c84(1);
    if (task->timer != 0) {
        func_0201e6e8(16 - task->timer, 14, task->timer * 2, 4);
    }
    Gfx_RequestLayerCopy(1);
    Screen_SetCurrent(saved027c);
    Screen_SetCurrentGfx(saved0280);
    if (task->timer == 0) {
        func_01ff8c84(1);
        Gfx_RequestLayerCopy(1);
        Task_ResumeScript(task);
    }
}
