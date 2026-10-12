#include "sdk/wl.h"

void WifiFlash_ReadLocked(u32 address, u32 size, u8 *dest);
void WifiFlash_ReadRadioBytes(u32 offset, u32 size, u8 *dest);

int WifiRf_TuneChannel(u16 channel, int fromFlash)
{
    void (*read)(u32, u32, u8 *);
    u16 saved;
    int power;
    int state;
    u32 value;
    u32 reg;
    u32 offset;
    u32 i;
    u32 index;

    if (fromFlash != 0) {
        read = WifiFlash_ReadLocked;
    } else {
        read = WifiFlash_ReadRadioBytes;
    }
    if (WifiParam_ChannelAllowed(channel) == 0) {
        return 5;
    }
    saved = WIFI_MAC_REGISTER(0x040);
    WIFI_MAC_REGISTER(0x040) = 0x8001;
    do {
        power = WIFI_MAC_REGISTER(0x03c) >> 8;
        state = WIFI_MAC_REGISTER(0x214);
    } while (power != 2 || (state != 0 && state != 9));

    WIFI_DRIVER_STATE->state.unk_92 = channel;
    switch (WIFI_DRIVER_STATE->rf.type) {
    case 2:
    case 5:
        value = 0;
        index = channel - 1;
        read(index * 6 + 0xf2, 3, (u8 *)&value);
        WifiRf_SendWord(value);
        read(index * 6 + 0xf5, 3, (u8 *)&value);
        WifiRf_SendWord(value);
        value = 0;
        if (WIFI_DRIVER_STATE->rf.unk_0c & 0x10000) {
            if (!(WIFI_DRIVER_STATE->rf.unk_0c & 0x8000)) {
                read(index + 0x154, 1, (u8 *)&value);
                value = WIFI_DRIVER_STATE->rf.unk_0c | ((value & 0x1f) << 10);
                WifiRf_SendWord(value);
            }
        } else {
            read(index + 0x146, 1, (u8 *)&value);
            WifiBaseband_Put(30, value);
        }
        break;
    case 3:
        offset = WIFI_DRIVER_STATE->rf.entryCount + 0xcf;
        // The original reads the u8 unk_08 as a halfword.
        for (i = 0; i < *(u16 *)&WIFI_DRIVER_STATE->rf.unk_08; i++) {
            value = 0;
            reg = 0;
            read(offset, 1, (u8 *)&reg);
            read(offset + channel, 1, (u8 *)&value);
            WifiBaseband_Put(reg, value);
            offset += 15;
        }
        for (i = 0; i < WIFI_DRIVER_STATE->rf.unk_06; i++) {
            value = 0;
            read(offset, 1, (u8 *)&value);
            value <<= 8;
            read(offset + channel, 1, (u8 *)&value);
            value |= 0x50000;
            WifiRf_SendWord(value);
            offset += 15;
        }
        break;
    }
    WIFI_MAC_REGISTER(0x040) = saved;
    WIFI_MAC_REGISTER(0x048) = 3;
    return 0;
}
