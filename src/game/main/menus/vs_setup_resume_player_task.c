#include "types.h"

void Task_ResumeByScript(void *script);

/* The task scripts of the four player slots; VsSetup_DestroyPlayerTasks ends them. */
extern u32 data_0215cec8[];
extern u32 data_0215cf00[];
extern u32 data_0215cf38[];
extern u32 data_0215cf70[];

/* Resumes the task of the player's slot (0-3). */
void VsSetup_ResumePlayerTask(int player)
{
    switch (player) {
    case 0:
        Task_ResumeByScript(data_0215cec8);
        break;
    case 1:
        Task_ResumeByScript(data_0215cf00);
        break;
    case 2:
        Task_ResumeByScript(data_0215cf38);
        break;
    default:
        Task_ResumeByScript(data_0215cf70);
        break;
    }
}
