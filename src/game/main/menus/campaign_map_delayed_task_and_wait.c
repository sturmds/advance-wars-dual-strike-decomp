#include "types.h"
#include "game/campaign_map.h"

void func_0204ecc4(void);
BOOL func_0204ec78(void);
void Task_ResumeScript(CampaignMapTask *task);

/* Script step like CampaignMap_StartTaskAndWait, but the task of
 * func_0204ecc4 starts after 16 frames. */
void CampaignMap_DelayedTaskAndWait(CampaignMapTask *task)
{
    if (task->step <= 16) {
        if (task->step == 16) {
            func_0204ecc4();
        }
        task->step++;
        return;
    }
    if (func_0204ec78()) {
        return;
    }
    task->step = 0;
    Task_ResumeScript(task);
}
