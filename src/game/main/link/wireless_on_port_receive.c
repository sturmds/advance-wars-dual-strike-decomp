#include "types.h"
#include "sdk/wm.h"

extern u32 gWirelessState;

void Link_ReceiveFromChild(u16 aid, void* data, u16 length);
void Link_ReceiveFromParent(void* data, u16 length);

/* Port receive callback. Data and disconnects go to Link_ReceiveFromChild with the
 * sender's aid when gWirelessState is 4 (looks like the parent side), or to
 * Link_ReceiveFromParent when it is 5 (child side). A disconnect passes no data. */
void Wireless_OnPortReceive(WMPortRecvCallback* cb) {
  if (cb->errcode != 0) {
    return;
  }
  switch (cb->state) {
    case WM_STATECODE_CONNECTED:
      return;
    case WM_STATECODE_PORT_RECV:
      switch (gWirelessState) {
        case 4:
          Link_ReceiveFromChild(cb->aid, cb->data, cb->length);
          return;
        case 5:
          Link_ReceiveFromParent(cb->data, cb->length);
          return;
      }
      return;
    case WM_STATECODE_DISCONNECTED:
      switch (gWirelessState) {
        case 4:
          Link_ReceiveFromChild(cb->aid, NULL, 0);
          return;
        case 5:
          Link_ReceiveFromParent(NULL, 0);
          return;
      }
      return;
  }
}
