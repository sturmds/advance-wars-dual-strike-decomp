#include "types.h"
#include "game/Unk_0215fe84.h"
#include "game/Unk_0229c960.h"

void func_020781b0(void *data, u32 size);

/* Sends a packet from the parent. A packet over the parent size limit counts as
 * done (dropped); false means the link is not ready for it yet. */
BOOL Wireless_SendParentData(void *data, u32 size)
{
    if (size > data_0229c960.parentMaxSize) {
        return TRUE;
    }
    if (data_0215fe84->unk_08 == 0) {
        return FALSE;
    }
    data_0215fe84->unk_08 = 0;
    func_020781b0(data, size);
    return TRUE;
}
