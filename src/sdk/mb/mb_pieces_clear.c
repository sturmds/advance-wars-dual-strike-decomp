#include "types.h"
#include "sdk/mb.h"

void MI_CpuFill8(void *dst, u8 value, u32 size);

/* Forgets the pieces received from child aid. */
void MbPieces_Clear(int aid)
{
    int child;

    if (gMbRecvPieces == NULL) {
        return;
    }
    child = aid - 1;
    MI_CpuFill8(gMbRecvPieces->data[child], 0, DLPLAY_FRAGMENT_TOTAL_BYTES);
    gMbRecvPieces->received[child] = 0;
}
