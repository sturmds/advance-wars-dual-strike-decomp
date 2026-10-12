#include "types.h"
#include "sdk/mb.h"

void MI_CpuFill8(void *dest, u8 value, u32 size);

void MbTask_InitInfo(DlPlayWorkerJob *info)
{
    MI_CpuFill8(info, 0, sizeof(DlPlayWorkerJob));
}
