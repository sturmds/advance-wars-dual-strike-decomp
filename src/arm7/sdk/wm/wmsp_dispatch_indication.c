#include "types.h"
#include "sdk/wmsp.h"

void Wmsp_IndicateAuthenticate(WlIndicationHeader *ind);
void Wmsp_IndicateDeauthenticate(WlDeauthIndication *ind);
void Wmsp_IndicateAssociate(WlAssocIndication *ind);
void Wmsp_IndicateReassociate(WlIndicationHeader *ind);
void Wmsp_IndicateDisassociate(WlIndicationHeader *ind);
void Wmsp_IndicateBeaconLost(WlIndicationHeader *ind);
void Wmsp_IndicateBeaconSent(WlIndicationHeader *ind);
void Wmsp_IndicateBeaconRecv(WlBeaconIndication *ind);
void Wmsp_IndicateDcfData(WlDataIndication *ind);
void Wmsp_IndicateMp(WlMpIndication *ind);
void func_arm7_027c14a8(WlMpEndIndication *ind);
void Wmsp_IndicateMpAck(WlMpAckIndication *ind);
void Wmsp_AutoDisconnectAll(WlIndicationHeader *ind);
void func_arm7_027c1a94(WlIndicationHeader *ind); /* empty */
void Wmsp_FreeIndication(WlIndicationHeader *ind);

/* Runs the handler for one WL indication, then frees it. Indications that
   arrive before WMSP is set up, or while WM is stopped, are ignored and
   not freed. */
void Wmsp_DispatchIndication(WlIndicationHeader *ind)
{
    if (func_arm7_027c05e0()->buf == NULL) {
        return;
    }
    if (func_arm7_027c05e0()->buf->status->state == WM_STATE_STOP) {
        return;
    }
    switch (ind->command) {
    case WIFI_HOSTMSG_PEER_AUTHED:
        Wmsp_IndicateAuthenticate(ind);
        break;
    case WIFI_HOSTMSG_PEER_UNAUTHED:
        Wmsp_IndicateDeauthenticate((WlDeauthIndication *)ind);
        break;
    case WIFI_HOSTMSG_PEER_JOINED:
        Wmsp_IndicateAssociate((WlAssocIndication *)ind);
        break;
    case WIFI_HOSTMSG_PEER_REJOINED:
        Wmsp_IndicateReassociate(ind);
        break;
    case WIFI_HOSTMSG_PEER_LEFT:
        Wmsp_IndicateDisassociate(ind);
        break;
    case WIFI_HOSTMSG_PARENT_GONE:
        Wmsp_IndicateBeaconLost(ind);
        break;
    case WIFI_HOSTMSG_BEACON_OUT:
        Wmsp_IndicateBeaconSent(ind);
        break;
    case WIFI_HOSTMSG_BEACON_HEARD:
        Wmsp_IndicateBeaconRecv((WlBeaconIndication *)ind);
        break;
    case WIFI_HOSTMSG_DATA_FRAME:
        Wmsp_IndicateDcfData((WlDataIndication *)ind);
        break;
    case WIFI_HOSTMSG_POLL_HEARD:
        Wmsp_IndicateMp((WlMpIndication *)ind);
        break;
    case WIFI_HOSTMSG_POLL_FINISHED:
        func_arm7_027c14a8((WlMpEndIndication *)ind);
        break;
    case WIFI_HOSTMSG_POLL_ACKED:
        Wmsp_IndicateMpAck((WlMpAckIndication *)ind);
        break;
    case WIFI_HOSTMSG_EVENT_BITS:
        Wmsp_AutoDisconnectAll(ind);
        break;
    default:
        func_arm7_027c1a94(ind);
        break;
    }
    Wmsp_FreeIndication(ind);
}
