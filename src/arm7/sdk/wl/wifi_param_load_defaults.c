#include "sdk/wl.h"

void WifiFlash_ReadRadioBytes(u32 offset, u32 size, u8 *dest);
int WifiParam_StoreOwnMac(const u16 *macAddress);
int WifiParam_StoreRetryLimit(u16 value);
int WifiParam_StoreAllowedChannels(u16 mask);
int WifiParam_StoreRole(u16 phase);
int WifiParam_StoreRateMode(u32 value);
int WifiParam_StoreWepMode(u32 value);
int func_arm7_027ca130(u32 value);
int WifiRam_StoreFourKeyBlocks(int src);
int func_arm7_027c9aa0(u32 value);
int WifiParam_SetSsidHidden(u32 enable);
int WifiParam_SetProbeNeedsSsid(u32 enable);
int WifiParam_StoreBeaconsPerRun(u32 value);
// The definitions of these two have a third, unused parameter.
int WifiParam_StorePostBeaconTime(u16 value, int writeReg);
int WifiBaseband_SetCcaParams(int reg13Value, int reg35Value);
int WifiParam_StoreSsidMask(const u16 *words);
int WifiParam_SetShortPreamble(u32 enable);
int func_arm7_027c9e50(int value);
int WifiParam_StoreRates(const WlRateSet *rates);
int func_arm7_027c933c(u32 value);
int WifiParam_SetAntennaSwitching(int bit4, int bit5);
int WifiParam_SetAntennaChoice(int bit3);
int func_arm7_027c9c84(u32 enable);
void Wifi_SeedLcg(u32 mul, u32 add);

extern u8 gWlDefaultWifiRamBlocks[80];
extern u16 gWlDefaultWords58[16];
extern WlRateSet gWlDefaultRateSet;

void WifiParam_LoadDefaults(void)
{
    u16 channels;
    u16 macAddress[3];

    WifiFlash_ReadRadioBytes(0x36, 6, (u8 *)macAddress);
    WifiFlash_ReadRadioBytes(0x3c, 2, (u8 *)&channels);
    WifiParam_StoreOwnMac(macAddress);
    WifiParam_StoreRetryLimit(7);
    WifiParam_StoreAllowedChannels(channels & 0x7ffe);
    WifiParam_StoreRole(2);
    WifiParam_StoreRateMode(0);
    WifiParam_StoreWepMode(0);
    func_arm7_027ca130(0);
    WifiRam_StoreFourKeyBlocks((int)gWlDefaultWifiRamBlocks);
    func_arm7_027c9aa0(500);
    WifiParam_SetSsidHidden(0);
    WifiParam_SetProbeNeedsSsid(0);
    WifiParam_StoreBeaconsPerRun(16);
    WifiParam_StorePostBeaconTime(0xffff, 0);
    WifiParam_StoreSsidMask(gWlDefaultWords58);
    WifiParam_SetShortPreamble(1);
    func_arm7_027c9e50(0);
    WifiParam_StoreRates(&gWlDefaultRateSet);
    WifiBaseband_SetCcaParams(0, 31);
    func_arm7_027c933c(5);
    WifiParam_SetAntennaSwitching(0, 0);
    WifiParam_SetAntennaChoice(0);
    func_arm7_027c9c84(0);
    Wifi_SeedLcg(WIFI_MAC_REGISTER(0x044) + (WIFI_MAC_REGISTER(0x044) << 8), WIFI_MAC_REGISTER(0x044));
    WIFI_DRIVER_STATE->state.unk_2c = 1;
}
