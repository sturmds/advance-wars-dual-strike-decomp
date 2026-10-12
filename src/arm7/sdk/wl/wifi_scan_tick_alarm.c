#include "sdk/wl.h"

/*
 * Alarm handler of WifiMlme_RunScan, every probeIntervalMs. Once the
 * channel time is used up it moves on to the next channel, or ends the scan
 * after the last one; otherwise the scan step sends another probe.
 */
void WifiScan_TickAlarm(void *arg)
{
    WlScanJob *job = &WIFI_ACTIVE_JOB_OF(WIFI_DRIVER_STATE)->scan;

    job->elapsedMs += job->probeIntervalMs;
    if (job->elapsedMs >= job->request->channelTimeMs) {
        if (job->channelIndex < 16) {
            job->state = 0x11;
        } else {
            job->state = 0x15;
        }
    }
    WifiTask_Schedule(2, 0);
}
