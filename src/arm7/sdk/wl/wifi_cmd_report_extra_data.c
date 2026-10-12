#include "sdk/mi.h"
#include "sdk/wl.h"

u8 WL_ReadByte(const void *addr);
void WL_WriteByte(void *addr, u32 value);

// Copies the data block that WlWork.unk_318 points to into the reply, as
// WifiHost_TellBeaconHeard does into its indication.
int WifiCmd_ReportExtraData(WlRequest *request, WlDataReply *reply)
{
    WlWork *work;

    if (reply->length > 1) {
        reply->dataLength = WIFI_DRIVER_STATE->unk_318.dataLength;
    }
    work = WIFI_DRIVER_STATE;
    if (work->unk_318.dataLength > (reply->length - 2) * 2) {
        return WIFI_ANSWER_ROOM_SHORT;
    }
    if (reply->dataLength != 0) {
        if (work->unk_318.flags & 1) {
            const u8 *src = work->unk_318.data + 1;
            u8 *dest = reply->data;
            u32 i;

            for (i = 0; i < reply->dataLength; i++) {
                WL_WriteByte(dest, WL_ReadByte(src));
                dest++;
                src++;
            }
        } else {
            Mem_CopyHalfwords_Arm7(work->unk_318.data, reply->data, reply->dataLength + 1);
        }
    }
    /* 2 halfwords before the data, then the data rounded up to halfwords */
    reply->length = _s32_div_f(reply->dataLength + 1, 2) + 2;
    return WIFI_ANSWER_OK;
}
