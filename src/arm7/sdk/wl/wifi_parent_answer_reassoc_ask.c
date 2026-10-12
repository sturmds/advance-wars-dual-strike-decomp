#include "types.h"
#include "sdk/mi.h"
#include "sdk/wl.h"

u16 func_arm7_027cb140(u32 station);                   /* station state */
void func_arm7_027cb2d0(u32 station, u32 rates);       /* sets WlStation.unk_10 */
void func_arm7_027cb2e8(u32 station, u32 capability);  /* sets the station's capability word */
void WifiStation_ChangeState(u16 station, u16 state);
WlTxFrame *WifiFrame_NewDeauthentication(const u16 *dest, u32 reason);
WlTxFrame *WifiFrame_NewReassocAnswer(u16 station, u16 status, const u8 *element);
void WifiTx_EnqueueMgmt(WlTxFrame *frame);
void func_arm7_027d1d84(WlElements *elements);

/*
 * Reassociation request, parent side. A station that has not authenticated
 * gets a deauthentication; an associated one is first moved back to
 * authenticated. The response refuses capability bits that do not fit
 * (privacy must agree with WlState.unk_08), a failed SSID check and rates
 * that do not fit.
 */
void WifiParent_AnswerReassocAsk(WlRxFrame *frame)
{
    volatile u32 zero; /* needed to match: the fill value goes through the stack */
    WlElements elements;
    WlWork *work = WIFI_DRIVER_STATE;
    WlCapability *capability;
    u32 bodyLength;
    u16 station;
    u16 status;
    WlTxFrame *response;

    bodyLength = frame->bodyLength;
    /* body: capability, listen interval, current AP, then the elements */
    capability = (WlCapability *)frame->body;
    if (bodyLength <= 10 || work->state.unk_24 != 1) {
        return;
    }
    station = frame->station;
    if (func_arm7_027cb140(station) < WIFI_PEER_AUTH_DONE) {
        response = WifiFrame_NewDeauthentication(frame->addr2, WIFI_REASON_UNAUTHED_SENDER);
        if (response != NULL) {
            WifiTx_EnqueueMgmt(response);
        }
        return;
    }
    if (func_arm7_027cb140(station) == WIFI_PEER_ASSOCIATED) {
        WifiStation_ChangeState(station, WIFI_PEER_AUTH_DONE);
        WifiHost_TellDisassociated((const u8 *)frame->addr2, 1);
    }
    zero = 0;
    Mem_FillWords_Arm7(zero, &elements, sizeof(elements));
    elements.data = (const u8 *)capability + 10;
    elements.length = bodyLength - 10;
    elements.unk_0c = 0x800;
    func_arm7_027d1d84(&elements);
    if ((capability->raw & WIFI_CAPS_PARENT_REJECTS) != 0 ||
        (work->state.unk_08 == 0 && capability->privacy == 1) ||
        (work->state.unk_08 != 0 && capability->privacy == 0)) {
        status = WIFI_IEEE_STATUS_CAPS_UNSUPPORTED;
    } else {
        func_arm7_027cb2e8(station, capability->raw);
        if (!(elements.result & WIFI_IE_FITS_SSID)) {
            status = WIFI_IEEE_STATUS_FAILURE;
        } else if (!(elements.result & WIFI_IE_FITS_RATES)) {
            status = WIFI_IEEE_STATUS_RATES_UNSUPPORTED;
        } else {
            func_arm7_027cb2d0(station, elements.supportedRates);
            status = WIFI_IEEE_STATUS_OK;
        }
    }
    response = WifiFrame_NewReassocAnswer(station, status, elements.unk_18);
    if (response != NULL) {
        WifiTx_EnqueueMgmt(response);
    }
}
