#include "types.h"
#include "sdk/mb.h"

void MI_CpuFill8(void *dest, u8 value, u32 size);

/* Sets and clears the buffer the parent collects child request pieces in. */
void MbPieces_Init(DlPlayFragmentStore *buffer)
{
    gMbRecvPieces = buffer;
    MI_CpuFill8(buffer, 0, sizeof(DlPlayFragmentStore));
}
