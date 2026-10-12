#include "sdk/wl.h"

void WifiTx_DropStationFrames(u32 station);
void Wifi_SwitchMode(int mode);
void WifiStation_ChangeState(u16 station, u16 state);

/*
 * Task 10: counts down each station's timeout (0 and 0xffff never
 * expire). When one runs out, its queued frames are flushed and the host is
 * told: a parent reports a station that was past unauthenticated, a child
 * reports its parent and stops the link, both with 802.11 reason code 1
 * (unspecified). The entry is then freed.
 */
void WifiTask_AgeStations(void)
{
    WlWork *work = WIFI_DRIVER_STATE;
    WlStation *station = &WIFI_PEER_ARRAY[1];
    u32 index;
    u32 seen; /* declaration order needed to match */
    u32 count = work->stationBits.stationCount;

    seen = 0;
    /* Needed to match: the table count is read through work on every pass. */
    for (index = 1; index < ((WlStationTable *)&work->limits.unk_f0)->count; index++, station++) {
        if (station->state == 0) {
            continue;
        }
        if (station->timeout != 0 && station->timeout != 0xffff) {
            station->timeout--;
            if (station->timeout == 0) {
                if (station->state >= WIFI_PEER_KNOWN_ONLY) {
                    WifiTx_DropStationFrames(index);
                    if (work->state.unk_24 == 1) {
                        if (station->state > WIFI_PEER_KNOWN_ONLY) {
                            WifiHost_TellDeauthenticated((const u8 *)station->macAddress, 1);
                        }
                    } else if (index == WIFI_LINK_VIEW_OF(work)->unk_88) {
                        Wifi_SwitchMode(WIFI_MODE_READY);
                        WifiLink_DropAssocId();
                        WifiHost_TellDeauthenticated((const u8 *)station->macAddress, 1);
                    }
                    WifiStation_ChangeState(index, WIFI_PEER_KNOWN_ONLY);
                }
                station->state = 0;
                work->stationBits.stationCount--;
            }
        }
        seen++;
        if (seen >= count) {
            break;
        }
    }
}
