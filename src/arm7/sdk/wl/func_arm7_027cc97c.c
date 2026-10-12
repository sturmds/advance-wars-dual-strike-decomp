#include "sdk/wl.h"

extern u16 data_arm7_027d6680[3];

u32 WifiFlash_ValidateRadioBlock(u32 *crcs);
void func_arm7_027c926c(int length, u8 *data);
void WifiParam_StoreRates(u16 *masks);
void WifiRf_TuneChannel(u16 channel, int arg);
void WifiParam_ChangeListenPeriod(int value);
void WifiParam_ChangeBeaconPeriod(int value);
void func_arm7_027c9b00(int length, u8 *ssid);
void func_arm7_027c9c1c(u16 *address);
void func_arm7_037f8e04(void);

/*
 * Checks a request of network settings (a 1-32 byte SSID, a channel, ranged
 * values and two 12-bit masks) and hands each one to its hardware setup
 * function. The address passed to func_arm7_027c9c1c is the broadcast address
 * (data_arm7_027d6680) in phase 0 and the own MAC address in phase 1; other
 * phases return 11.
 */
int func_arm7_027cc97c(WlBssSettings *request, WlReply *reply)
{
    WlLink *link = WIFI_LINK_VIEW_OF(WIFI_DRIVER_STATE);
    WlHead *head = WIFI_SESSION_HEAD_OF(WIFI_DRIVER_STATE);

    reply->status = 1;
    if (head->phase != 1 && head->phase != 0) {
        return 11;
    }
    if (link->mode != 0x20) {
        return 1;
    }
    if (request->ssidLength > 32) {
        return WIFI_ANSWER_OUT_OF_RANGE;
    }
    if (request->ssidLength == 0) {
        return WIFI_ANSWER_OUT_OF_RANGE;
    }
    if (request->unk_32 < 10) {
        return WIFI_ANSWER_OUT_OF_RANGE;
    }
    if (request->unk_32 > 1000) {
        return WIFI_ANSWER_OUT_OF_RANGE;
    }
    if (request->unk_34 == 0) {
        return WIFI_ANSWER_OUT_OF_RANGE;
    }
    if (request->unk_34 > 255) {
        return WIFI_ANSWER_OUT_OF_RANGE;
    }
    if (request->channel & 0xfff0) {
        return WIFI_ANSWER_OUT_OF_RANGE;
    }
    if (WifiParam_ChannelAllowed(request->channel) == 0) {
        return WIFI_ANSWER_OUT_OF_RANGE;
    }
    if (request->unk_38[0] == 0) {
        return WIFI_ANSWER_OUT_OF_RANGE;
    }
    if (request->unk_38[0] & ~0xfff) {
        return WIFI_ANSWER_OUT_OF_RANGE;
    }
    if (request->unk_38[1] == 0) {
        return WIFI_ANSWER_OUT_OF_RANGE;
    }
    if (request->unk_38[1] & ~0xfff) {
        return WIFI_ANSWER_OUT_OF_RANGE;
    }
    if (request->unk_3c > 128) {
        return WIFI_ANSWER_OUT_OF_RANGE;
    }
    if (WifiFlash_ValidateRadioBlock(NULL) != WIFI_RADIO_BLOCK_VALID) {
        return WIFI_ANSWER_FLASH_BLOCK_BAD;
    }
    if (head->phase == 0) {
        func_arm7_027c9c1c(data_arm7_027d6680);
    } else {
        func_arm7_027c9c1c(head->macAddress);
    }
    func_arm7_027c9b00(request->ssidLength, request->ssid);
    WifiParam_ChangeBeaconPeriod(request->unk_32);
    WifiParam_ChangeListenPeriod(request->unk_34);
    WifiRf_TuneChannel(request->channel, 0);
    WifiParam_StoreRates(request->unk_38);
    func_arm7_027c926c(request->unk_3c, request->unk_3e);
    link->unk_a4 = 0;
    func_arm7_037f8e04();
    return 0;
}
