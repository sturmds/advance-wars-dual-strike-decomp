#include "sdk/wl.h"
#include "sdk/mi.h"

u8 WL_ReadByte(const void *addr);
void WL_WriteByte(void *addr, u32 value);

int func_arm7_027c91c0(u32 length, const u8 *src)
{
    WlParams *params = WIFI_SETTINGS_BLOCK_OF(WIFI_DRIVER_STATE);
    u8 *dest;
    u32 i;

    if (length > 128) {
        return 4;
    }
    if (length != 0) {
        if (params->unk_a2 & 1) {
            dest = params->unk_9c;
            WL_WriteByte(dest, 0xff);
            dest++;
            for (i = 0; i < length; i++) {
                WL_WriteByte(dest, WL_ReadByte(src));
                dest++;
                src++;
            }
        } else {
            Mem_CopyHalfwords_Arm7(src, params->unk_9c, length + 1);
        }
    }
    params->unk_a0 = length;
    params->unk_a4 = 1;
    return 0;
}
