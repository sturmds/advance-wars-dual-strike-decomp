#include "sdk/wl.h"

u16 func_arm7_027c8d08(int channel);
u32 WifiFlash_ValidateRadioBlock(u32 *crcs);
void WifiFlash_ReadLocked(u32 address, u32 size, u8 *dest);
u16 func_arm7_027c8bcc(int reg);
int func_arm7_027c8b8c(int reg, int value);
void func_arm7_027c87b8(int a0);
void func_arm7_027c963c(int channel, int arg1);
void func_arm7_027c8714(void);
void WifiTest_SendPatternFrames(WlTestModeRequest *request);

// Starts or stops a test transmission on one channel. Before starting, it loads
// baseband register 1 from flash byte 0x65 when they differ.
int WifiCmd_RunRadioTest(WlTestModeRequest *request, WlReply *reply)
{
    WlLink *link = WIFI_LINK_VIEW_OF(WIFI_DRIVER_STATE);
    // needed to match: one variable, kept in memory because its address is
    // taken, holds the flash byte and later baseband register 2
    u32 regValue;
    u32 bbReg1;

    reply->status = 1;
    if ((link->mode & 0xf0) != 0x10) {
        return 1;
    }
    if (request->start > 1) {
        return 5;
    }
    if (request->rate != 10 && request->rate != 20) {
        return 5;
    }
    if (!func_arm7_027c8d08(request->channel)) {
        return 5;
    }
    if (request->signal > 4) {
        return 5;
    }
    if (WifiFlash_ValidateRadioBlock(NULL) != WIFI_RADIO_BLOCK_VALID) {
        return 14;
    }
    switch (request->start) {
    case 1:
        if (link->mode != WIFI_MODE_RADIO_ON) {
            return 1;
        }
        regValue = 0;
        WifiFlash_ReadLocked(0x65, 1, (u8 *)&regValue);
        bbReg1 = regValue;
        if (bbReg1 != func_arm7_027c8bcc(1)) {
            func_arm7_027c8b8c(1, bbReg1);
            func_arm7_027c87b8(5000);
        }
        switch (request->signal) {
        case 0:
        case 1:
        case 2:
            link->mode = WIFI_MODE_CARRIER_TEST;
            func_arm7_027c963c(request->channel, 1);
            WIFI_MAC_FORCE_POWER = 0x8000;
            func_arm7_027c87b8(1500);
            link->testRate = request->rate;
            regValue = func_arm7_027c8bcc(2);
            if (request->signal <= 1) {
                regValue |= 0x10;
            }
            WIFI_TEST_TX_RATE = request->rate;
            if (request->signal != 1) {
                WIFI_TEST_TX_PATTERN = request->signal;
            } else {
                regValue |= 0x20;
                WIFI_TEST_TX_PATTERN = 3;
            }
            func_arm7_027c8b8c(2, regValue);
            WIFI_TEST_TX_CONTROL = 0x823;
            break;
        case 3:
        case 4:
            WifiTest_SendPatternFrames(request);
            break;
        }
        break;
    case 0:
        if (link->mode == WIFI_MODE_CARRIER_TEST) {
            func_arm7_027c8714();
            WIFI_TEST_TX_CONTROL = 0;
            WIFI_TEST_TX_PATTERN = 1;
            WIFI_MAC_FORCE_POWER = 0;
            regValue = func_arm7_027c8bcc(2);
            regValue &= ~0x30;
            func_arm7_027c8b8c(2, regValue);
        } else if (link->mode == WIFI_MODE_FRAME_TEST) {
            link->testTxActive = 0;
            while (WIFI_MAC_RUN != 0) {
            }
            func_arm7_027c8b8c(6, link->savedBbReg6);
        } else {
            return 1;
        }
        link->mode = WIFI_MODE_RADIO_ON;
        break;
    }
    return 0;
}
