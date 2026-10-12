#include "sdk/wl.h"

void func_arm7_027c84f4(void *dest, const void *src, u32 halfwords);

/* Copies a 36-byte frame header and the body right after it (frame not protected). */
void WifiTx_CopyHeaderAndBody(void *dest, const void *header, const void *body, u32 bodySize)
{
    func_arm7_027c84f4(dest, header, 18);
    if (bodySize != 0) {
        func_arm7_027c84f4((u8 *)dest + 36, body, (bodySize + 1) >> 1);
    }
}
