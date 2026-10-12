#include "types.h"
#include "sdk/mi.h"
#include "sdk/wl.h"

WlTxFrame* WifiFrame_NewProbeAnswer(const u16* dest);
void WifiTx_EnqueueMgmt(WlTxFrame* frame);
void WifiRx_ScanElements(WlElements* elements);

/*
 * Probe request received on the parent side: answers with a probe response
 * when the request's elements match this network (SSID check).
 */
void WifiParent_AnswerProbe(WlRxFrame* frame) {
  volatile u32
      zero; /* needed to match: the fill value goes through the stack */
  WlElements elements;
  WlTxFrame* response;

  if ((frame->addr3[0] & 1) || (frame->unk_08 & 0x8000)) {
    zero = 0;
    Mem_FillWords_Arm7(zero, &elements, sizeof(elements));
    elements.data = (const u8*)frame->body;
    elements.length = frame->bodyLength;
    if (!WIFI_DRIVER_STATE->state.flags.bit1) {
      elements.present = 0x800;
    }
    WifiRx_ScanElements(&elements);
    if ((elements.result & WIFI_IE_FITS_SSID) == WIFI_IE_FITS_SSID) {
      response = WifiFrame_NewProbeAnswer(frame->addr2);
      if (response != NULL) {
        WifiTx_EnqueueMgmt(response);
      }
    }
  }
}
