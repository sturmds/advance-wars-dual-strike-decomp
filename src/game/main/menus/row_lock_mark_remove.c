#include "types.h"

void Task_ResumeByScript(void *script);
extern u32 data_02160ed0[];
extern u32 data_02160f00[];
extern u32 data_02160f30[];
extern u32 data_02160f60[];

/* Resumes the task that runs the script of team row 1 to 4. */
void RowLockMark_Remove(int row)
{
    switch (row) {
    case 1:
        Task_ResumeByScript(data_02160ed0);
        break;
    case 2:
        Task_ResumeByScript(data_02160f00);
        break;
    case 3:
        Task_ResumeByScript(data_02160f30);
        break;
    case 4:
        Task_ResumeByScript(data_02160f60);
        break;
    }
}
