#include "types.h"
#include "sdk/mb.h"

extern DlPlayFragmentStore *data_022acf04;

void MI_CpuFill8(void *dst, u8 value, u32 size);

/* Forgets the pieces received from child aid. */
void MbPieces_Clear(int aid)
{
    int child;

    if (data_022acf04 == NULL) {
        return;
    }
    child = aid - 1;
    MI_CpuFill8(data_022acf04->data[child], 0, DLPLAY_FRAGMENT_TOTAL_BYTES);
    data_022acf04->received[child] = 0;
}
