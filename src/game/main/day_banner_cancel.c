#include "types.h"
#include "game/task.h"

void func_01ff8c84(int layer);
void Gfx_RequestLayerCopy(int layer);
void Task_DestroyByScript(void *script);

extern TaskScriptCmd data_0215bf54[];

/* Script step of the day banner's early-exit script: clears layer 0, asks for
 * its copy to VRAM and destroys the day banner task. */
void DayBanner_Cancel(void)
{
    func_01ff8c84(0);
    Gfx_RequestLayerCopy(0);
    Task_DestroyByScript(data_0215bf54);
}
