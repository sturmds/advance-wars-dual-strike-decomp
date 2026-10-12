#include "sdk/wl.h"

/* Called with two arguments here: the original leaves r2 alone. The
   definition's third parameter is unused. */
int WifiBaseband_SetCcaParams(int reg13Value, int reg35Value);
u8 WL_ReadByte(const void *addr);
void Wifi_ArmOneShotMs(u32 milliseconds, OSAlarmHandler handler);
int func_arm7_027c94ac(int value);
void WifiRf_TuneChannel(u16 channel, int arg);
void func_arm7_027cbfc8(void);
u32 WifiFlash_ValidateRadioBlock(u32 *crcs);
void func_arm7_037f8e04(void);
void func_arm7_037f92c8(void);

/*
 * One step of the channel measurement started by WifiCmd_StartChannelSurvey. For each
 * channel of the request it samples the busy bit of WIFI_MAC_REGISTER(0x19c) for
 * request->sampleMs and stores channel | ratio << 8 in the reply, where ratio
 * is 0 when no sample was busy, else the busy percent + 1, at most 100.
 * States: 0x80 start, 0x81 next channel, 0x82 sampling (the
 * alarm moves it to 0x83), 0x83 store the result, 0x84 restore and reply.
 */
void WifiMlme_RunChannelSurvey(void)
{
    WlLink *link = WIFI_LINK_VIEW_OF(WIFI_DRIVER_STATE);
    WlMeasureChannelJob *job = &WIFI_ACTIVE_JOB_OF(WIFI_DRIVER_STATE)->measure;
    u32 channel;
    u32 ratio;
    u32 i;

    switch (job->state) {
    case 0x80:
        job->index = 0;
        job->savedReg13 = WifiBaseband_Get(0x13);
        job->savedReg35 = WifiBaseband_Get(0x35);
        WifiBaseband_SetCcaParams(job->request->reg13Value, job->request->reg35Value);
        link->unk_0c = 4;
        job->result = 0;
    case 0x81:
        job->sampleCount = 0;
        job->busyTotal = 0;
        channel = WL_ReadByte(&job->request->channels[job->index]);
        if (channel == 0 || job->index >= 16) {
            job->state = 0x84;
            break;
        }
        if (WifiFlash_ValidateRadioBlock(NULL) != WIFI_RADIO_BLOCK_VALID) {
            job->result = WIFI_ANSWER_FLASH_BLOCK_BAD;
            job->state = 0x84;
            break;
        }
        if (job->state == 0x80) {
            WifiRf_TuneChannel(channel, 0);
            func_arm7_037f8e04();
            job->savedReg040 = WIFI_MAC_REGISTER(0x40);
            func_arm7_027c94ac(0x8000);
        } else {
            WifiRf_TuneChannel(channel, 0);
        }
        job->state = 0x82;
        Wifi_ArmOneShotMs(job->request->sampleMs, (OSAlarmHandler)func_arm7_027cbfc8);
    case 0x82:
        job->sampleCount++;
        if (WIFI_MAC_REGISTER(0x19c) & 1) {
            job->busyTotal += 100;
        }
        break;
    case 0x83:
        channel = WL_ReadByte(&job->request->channels[job->index]);
        ratio = 0;
        if (job->sampleCount != 0 && job->busyTotal != 0) {
            ratio = job->busyTotal / job->sampleCount + 1;
            if (ratio > 100) {
                ratio = 100;
            }
        }
        job->reply->words[job->index] = channel | (ratio << 8);
        job->index++;
        job->state = 0x81;
        break;
    case 0x84:
        func_arm7_037f92c8();
        link->unk_0c = WIFI_DRIVER_STATE->state.phase;
        WifiBaseband_Put(0x13, job->savedReg13);
        WifiBaseband_Put(0x35, job->savedReg35);
        func_arm7_027c94ac(job->savedReg040);
        job->reply->unk_04 = job->result;
        job->state = 0;
        for (i = job->index; i < 16; i++) {
            job->reply->words[i] = 0;
        }
        WifiHost_ReturnCurrentRequest();
        break;
    }
    if (job->state != 0) {
        WifiTask_Schedule(2, 5);
    }
}
