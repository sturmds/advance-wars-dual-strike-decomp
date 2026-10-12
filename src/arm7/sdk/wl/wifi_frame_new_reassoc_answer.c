#include "sdk/wl.h"

void WifiTx_FillHeader(WlTxFrame *frame, const u16 *dest);
u32 WifiFrame_PutRatesElement(u8 *dest);
u32 WifiStation_GiveAssocId(u16 station);
u8 WL_ReadByte(const void *addr);
void WL_WriteByte(void *addr, u32 value);

// Allocates a buffer and builds a reassociation response to a station. On
// status 0 it hands the station an AID, or answers WIFI_IEEE_STATUS_AP_FULL
// when none is left. The element (or an empty one when element is NULL) is
// copied after the end of the frame, outside its length. Returns NULL when no
// buffer is free.
WlTxFrame *WifiFrame_NewReassocAnswer(u16 station, u16 status, const u8 *element)
{
    /* needed to match: this declaration order */
    u32 i;
    WlTxFrame *frame;
    u8 *end;
    u32 length;
    WlTxBuffer *buffer;
    u32 aid;

    buffer = (WlTxBuffer *)WifiBuffer_New(&WIFI_DRIVER_STATE->unk_15c, 96);
    if (buffer == NULL) {
        WifiHost_RaiseEventBits(WIFI_EVT_TX_BUILD_FAILED);
        return (WlTxFrame *)buffer; /* needed to match: NULL, but returned from the register */
    }
    buffer->unk_0c = WIFI_DRIVER_BUILT_FRAME;
    frame = &buffer->frame;
    if (status == 0) {
        aid = WifiStation_GiveAssocId(station);
        if (aid == 0) {
            status = WIFI_IEEE_STATUS_AP_FULL;
        }
    } else {
        aid = 0;
    }
    WifiTx_FillHeader(frame, WifiStation_MacOf(station));
    WIFI_MANAGEMENT_FIELDS_OF(frame)->assocResponse.capability = WIFI_DRIVER_STATE->state.capability;
    WIFI_MANAGEMENT_FIELDS_OF(frame)->assocResponse.status = status;
    WIFI_MANAGEMENT_FIELDS_OF(frame)->assocResponse.aid = aid | WIFI_ASSOC_ID_TOP_BITS;
    frame->bodyLength = WifiFrame_PutRatesElement(WIFI_MANAGEMENT_FIELDS_OF(frame)->assocResponse.elements) + 6;
    frame->length = frame->bodyLength + WIFI_HEADER_PLUS_CHECKSUM;
    frame->frameControl = WIFI_FRAMECTL_REASSOC_ANSWER;

    end = (u8 *)&frame->frameControl + frame->length;
    if (element != NULL) {
        length = WL_ReadByte(element + 1);
        WL_WriteByte(end, WL_ReadByte(element));
        WL_WriteByte(end + 1, (u8)length);
        end += 2;
        for (i = 0; i < length; i++) {
            WL_WriteByte(end, WL_ReadByte(element + 2 + i));
            end++;
        }
    } else {
        WL_WriteByte(end, 0);
        WL_WriteByte(end + 1, 0);
    }
    return frame;
}
