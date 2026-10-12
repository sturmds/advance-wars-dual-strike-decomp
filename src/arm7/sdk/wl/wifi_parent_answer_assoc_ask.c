#include "types.h"
#include "sdk/mi.h"
#include "sdk/wl.h"

void WifiStation_ChangeState(u16 station, u16 state);
WlTxFrame* WifiFrame_NewDeauthentication(const u16* dest, u32 reason);
WlTxFrame* WifiFrame_NewAssocAnswer(u16 station, u16 status, const u8* element);
void WifiTx_EnqueueMgmt(WlTxFrame* frame);
void WifiRx_ScanElements(WlElements* elements);

/*
 * Association request, parent side. An unknown station or one that has not
 * authenticated gets a deauthentication; an associated one is first moved
 * back to authenticated. The response refuses capability bits that do not
 * fit (ESS must be set, privacy must agree with WlState.unk_08, short
 * preamble is needed when option bit 2 is set), a failed SSID check and
 * rates that do not fit.
 */
void WifiParent_AnswerAssocAsk(WlRxFrame* frame) {
  volatile u32
      zero; /* needed to match: the fill value goes through the stack */
  WlElements elements;
  WlWork* work = WIFI_DRIVER_STATE;
  WlStateView* view = WIFI_FROM_STATION_TABLE_OF(work);
  WlCapability* capability;
  u32 bodyLength;
  u16 station;
  u16 status;
  WlTxFrame* response;

  bodyLength = frame->bodyLength;
  /* body: capability, listen interval, then the elements */
  capability = (WlCapability*)frame->body;
  if (bodyLength <= 4 || work->state.unk_24 != 1) {
    return;
  }
  station = frame->station;
  if (station == 0 || WifiStation_StateOf(station) < WIFI_PEER_AUTH_DONE) {
    response = WifiFrame_NewDeauthentication(frame->addr2,
                                             WIFI_REASON_UNAUTHED_SENDER);
    if (response != NULL) {
      WifiTx_EnqueueMgmt(response);
    }
    return;
  }
  if (WifiStation_StateOf(station) == WIFI_PEER_ASSOCIATED) {
    WifiStation_ChangeState(station, WIFI_PEER_AUTH_DONE);
    WifiHost_TellDisassociated((const u8*)frame->addr2, 1);
  }
  zero = 0;
  Mem_FillWords_Arm7(zero, &elements, sizeof(elements));
  elements.data = (const u8*)capability + 4;
  elements.length = bodyLength - 4;
  WifiRx_ScanElements(&elements);
  if ((capability->raw & WIFI_CAPS_PARENT_REJECTS) != 0 || !capability->ess ||
      (view->state.unk_08 == 0 && capability->privacy == 1) ||
      (view->state.unk_08 != 0 && capability->privacy == 0) ||
      (view->state.flags.bit2 == 1 && !capability->shortPreamble)) {
    status = WIFI_IEEE_STATUS_CAPS_UNSUPPORTED;
  } else {
    WifiStation_StoreCapability(station, capability->raw);
    if (!(elements.result & WIFI_IE_FITS_SSID)) {
      status = WIFI_IEEE_STATUS_FAILURE;
    } else if (!(elements.result & WIFI_IE_FITS_RATES)) {
      status = WIFI_IEEE_STATUS_RATES_UNSUPPORTED;
    } else {
      WifiStation_StoreRates(station, elements.supportedRates);
      status = WIFI_IEEE_STATUS_OK;
    }
  }
  response = WifiFrame_NewAssocAnswer(station, status, elements.ssid);
  if (response != NULL) {
    WifiTx_EnqueueMgmt(response);
  }
}
