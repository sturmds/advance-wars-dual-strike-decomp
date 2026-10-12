#include "types.h"
#include "sdk/mi.h"
#include "sdk/wl.h"

u8 WL_ReadByte(const void *addr);
void WL_WriteByte(void *addr, u8 value);
void func_arm7_027c90ec(void *dest, const void *src); /* copies a 6-byte MAC address */

/*
 * Sends indication 0x8d: two bytes and a MAC address from info, then the data
 * block that WlWork.unk_318 points to. Returns 0 when no buffer is left.
 */
int WifiHost_TellBeaconHeard(Unk_027cb9e4 *info)
{
    WlWork *work = WIFI_DRIVER_STATE;
    WlUnk318 *unk = &work->unk_318;
    WlIndication *ind;

    ind = (WlIndication *)WifiBuffer_New(&work->unk_15c, work->unk_318.dataLength + 0x3e);
    if (ind == NULL) {
        WifiHost_RaiseEventBits(WIFI_EVT_NOTE_DROPPED);
        return 0;
    }
    ind->command = 0x8d;
    /* 22 halfwords before the data, then the data rounded up to halfwords */
    ind->length = (s32)func_arm7_03806d4c(unk->dataLength + 1, 2) + 22;
    WL_WriteByte(&ind->ind8d.unk_1f, info->unk_12);
    WL_WriteByte(&ind->ind8d.unk_1e, info->unk_0e);
    func_arm7_027c90ec(ind->ind8d.mac, info->mac);
    ind->ind8d.dataLength = unk->dataLength;
    if (ind->ind8d.dataLength != 0) {
        if (unk->flags & 1) {
            u32 i;
            const u8 *src = unk->data + 1;
            u8 *dest = ind->ind8d.data;

            for (i = 0; i < ind->ind8d.dataLength; i++) {
                WL_WriteByte(dest, WL_ReadByte(src));
                src++;
                dest++;
            }
        } else {
            Mem_CopyHalfwords_Arm7(unk->data, ind->ind8d.data, ind->ind8d.dataLength + 1);
        }
    }
    WifiBuffer_GiveToHost(&WIFI_DRIVER_STATE->unk_15c, &ind->buffer);
    return 1;
}
