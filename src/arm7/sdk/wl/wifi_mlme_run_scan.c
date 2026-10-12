#include "sdk/wl.h"

u8 WL_ReadByte(const void *addr);
void Wifi_ArmOneShotMs(u32 milliseconds, OSAlarmHandler handler);
void WifiScan_TickAlarm(void *arg);
void WifiRf_TuneChannel(u16 channel, int arg);
void *WifiFrame_NewProbeAsk(u16 *peer);
void WifiTx_EnqueueMgmt(void *frame);
u32 WifiFlash_ValidateRadioBlock(u32 *crcs);
void func_arm7_037f8e04(void);
void Wifi_CloseSession(void);
void Wifi_SwitchMode(int arg);

/*
 * Scan request, run as WL task 0. It spends request->channelTimeMs on each
 * channel of the request. Unless request->unk_38 is set it sends a probe
 * request (frame control 0x40) every quarter of that time, at least every
 * 10 ms. States: 0x10 start, 0x11 next channel, 0x13 send a probe and wait
 * for WifiScan_TickAlarm, 0x15 end and reply.
 */
void WifiMlme_RunScan(void)
{
    WlScanJob *job = &WIFI_ACTIVE_JOB_OF(WIFI_DRIVER_STATE)->scan;
    WlLink *link = WIFI_LINK_VIEW_OF(WIFI_DRIVER_STATE);
    BOOL requeue = FALSE;
    u16 channel;
    void *frame;

    switch (job->state) {
    case 0x10:
        Wifi_SwitchMode(0x20);
        link->unk_0c = 2;
        job->reply->words[0] = 0;
        job->reply->value = 0;
        job->channelIndex = 0;
        job->unk_08 = 0;
        if (job->request->unk_38 == 0) {
            job->probeIntervalMs = func_arm7_03806d4c(job->request->channelTimeMs + 3, 4);
            if (job->probeIntervalMs < 10) {
                job->probeIntervalMs = 10;
            }
        } else {
            job->probeIntervalMs = job->request->channelTimeMs;
        }
        job->reply->unk_04 = WIFI_ANSWER_OK;
    case 0x11:
        channel = WL_ReadByte(&job->request->channels[job->channelIndex]);
        if (channel == 0) {
            job->state = 0x15;
            requeue = TRUE;
            break;
        }
        job->channelIndex++;
        job->elapsedMs = 0;
        if (WifiFlash_ValidateRadioBlock(NULL) != WIFI_RADIO_BLOCK_VALID) {
            job->reply->unk_04 = WIFI_ANSWER_FLASH_BLOCK_BAD;
            job->state = 0x15;
            requeue = TRUE;
            break;
        }
        if (job->state == 0x10) {
            WifiRf_TuneChannel(channel, 0);
            func_arm7_037f8e04();
        } else {
            WifiRf_TuneChannel(channel, 0);
        }
        job->state = 0x12;
    case 0x12:
    case 0x13:
        job->state = 0x13;
        if (job->request->unk_38 == 0) {
            frame = WifiFrame_NewProbeAsk(job->request->peer);
            if (frame == NULL) {
                job->reply->unk_04 = WIFI_ANSWER_NO_MEMORY;
                job->state = 0x15;
                requeue = TRUE;
                break;
            }
            WifiTx_EnqueueMgmt(frame);
        }
        Wifi_ArmOneShotMs(job->probeIntervalMs, WifiScan_TickAlarm);
        break;
    case 0x15:
        job->state = 0;
        Wifi_CloseSession();
        link->unk_0c = WIFI_DRIVER_STATE->state.phase;
        WifiHost_ReturnCurrentRequest();
        break;
    }
    if (requeue) {
        WifiTask_Schedule(2, 0);
    }
}
