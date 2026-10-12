#include "sdk/wl.h"

void func_arm7_027c84f4(void *dest, const void *src, u32 halfwords);

/*
 * Copies a 36-byte frame header, skips 4 bytes, then copies the body: the
 * caller uses this one when frame control bit 0x4000 (protected) is set.
 */
void WifiTx_CopyWithWepGap(void *dest, const void *header, const void *body, u32 bodySize)
{
    func_arm7_027c84f4(dest, header, 18);
    if (bodySize != 0) {
        func_arm7_027c84f4((u8 *)dest + 40, body, (bodySize + 1) >> 1);
    }
}
