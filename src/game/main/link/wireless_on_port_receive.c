#include "types.h"
#include "sdk/wm.h"

extern u32 data_0229c88c;

void func_0207acc0(u16 aid, void *data, u16 length);
void Link_ReceiveFromParent(void *data, u16 length);

/* Port receive callback. Data and disconnects go to func_0207acc0 with the
 * sender's aid when data_0229c88c is 4 (looks like the parent side), or to
 * Link_ReceiveFromParent when it is 5 (child side). A disconnect passes no data. */
void Wireless_OnPortReceive(WMPortRecvCallback *cb)
{
    if (cb->errcode != 0) {
        return;
    }
    switch (cb->state) {
    case WM_STATECODE_CONNECTED:
        return;
    case WM_STATECODE_PORT_RECV:
        switch (data_0229c88c) {
        case 4:
            func_0207acc0(cb->aid, cb->data, cb->length);
            return;
        case 5:
            Link_ReceiveFromParent(cb->data, cb->length);
            return;
        }
        return;
    case WM_STATECODE_DISCONNECTED:
        switch (data_0229c88c) {
        case 4:
            func_0207acc0(cb->aid, NULL, 0);
            return;
        case 5:
            Link_ReceiveFromParent(NULL, 0);
            return;
        }
        return;
    }
}
