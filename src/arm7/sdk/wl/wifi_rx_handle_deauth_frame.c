#include "types.h"
#include "sdk/wl.h"

u16 func_arm7_027cb140(u32 station);   /* station state */
void WifiStation_ChangeState(u16 station, u16 state);
void WifiTx_DropStationFrames(u32 station);  /* ends the frames queued for the station */
void Wifi_SwitchMode(int mode);

/*
 * Deauthentication received. Parent side: an authenticated station goes
 * back to unauthenticated. Child side: when it comes from our parent, the
 * link goes back to WIFI_MODE_READY. Both tell the host with the reason code.
 */
void WifiRx_HandleDeauthFrame(WlRxFrame *frame)
{
    WlLink *link = WIFI_LINK_VIEW_OF(WIFI_DRIVER_STATE);
    u32 station = frame->station;

    switch (link->unk_0c) {
    case 1:
        if (func_arm7_027cb140(station) > WIFI_PEER_KNOWN_ONLY) {
            WifiStation_ChangeState(station, WIFI_PEER_KNOWN_ONLY);
            WifiHost_TellDeauthenticated((const u8 *)frame->addr2, frame->body[0]);
            WifiTx_DropStationFrames(station);
        }
        break;
    case 2:
    case 3:
        if (link->mode > WIFI_MODE_READY && WifiMac_Equal(frame->addr2, link->currentAp)) {
            Wifi_SwitchMode(WIFI_MODE_READY);
            WifiLink_DropAssocId();
            WifiHost_TellDeauthenticated((const u8 *)frame->addr2, frame->body[0]);
        }
        break;
    }
}
