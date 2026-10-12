#include "sdk/wl.h"

u16 func_arm7_027cb140(u32 station); /* the station's state */
void WifiStation_ChangeState(u16 station, u16 state);
void func_arm7_027d0424(u32 station);
WlTxFrame *WifiFrame_NewDisassociation(const u16 *dest, u32 reason);
WlTxFrame *WifiFrame_NewDeauthentication(const u16 *dest, u32 reason);
void WifiTx_EnqueueMgmt(void *frame);
void Wifi_SwitchMode(int mode); /* switches WlLink.mode */

/*
 * Handles a received disassociation frame. In WlLink.unk_0c 1 the sender is
 * one of our stations: an associated one (state 0x40) goes back to 0x30 and
 * the host is told; any other gets a disassociation (state 0x30) or a
 * deauthentication with reason 7 (frame from a station that is not
 * associated). In 2 and 3, a disassociation from our AP while associated
 * (mode 0x40) switches to mode 0x30.
 */
void WifiRx_HandleDisassocFrame(WlRxFrame *frame)
{
    WlLink *link = WIFI_LINK_VIEW_OF(WIFI_DRIVER_STATE);
    u32 station = frame->station;
    u16 state;
    WlTxFrame *reply;

    switch (link->unk_0c) {
    case 1:
        state = func_arm7_027cb140(station);
        if (state == WIFI_PEER_ASSOCIATED) {
            WifiStation_ChangeState(station, 0x30);
            WifiHost_TellDisassociated((const u8 *)frame->addr2, frame->body[0]);
            func_arm7_027d0424(station);
        } else {
            if (state == 0x30) {
                reply = WifiFrame_NewDisassociation(frame->addr2, 7);
            } else {
                reply = WifiFrame_NewDeauthentication(frame->addr2, 7);
            }
            if (reply != NULL) {
                WifiTx_EnqueueMgmt(reply);
            }
        }
        break;
    case 2:
    case 3:
        if (link->mode == 0x40 && WifiMac_Equal(frame->addr2, link->currentAp)) {
            Wifi_SwitchMode(0x30);
            WifiLink_DropAssocId();
            WifiHost_TellDisassociated((const u8 *)frame->addr2, frame->body[0]);
        }
        break;
    }
}
