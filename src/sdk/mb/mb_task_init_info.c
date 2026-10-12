#include "types.h"
#include "sdk/mb.h"

void MI_CpuFill8(void* dest, u32 data, u32 size);

void MbTask_InitInfo(DlPlayWorkerJob* info) {
  MI_CpuFill8(info, 0, sizeof(DlPlayWorkerJob));
}
