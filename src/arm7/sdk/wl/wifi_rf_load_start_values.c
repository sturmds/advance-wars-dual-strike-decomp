#include "sdk/wl.h"

extern const u16 gWlFirmwareRegOffsets[16];

void WifiFlash_ReadRadioBytes(u32 offset, u32 size, void *dest);
void func_arm7_027c8b64(u32 entry);

void WifiRf_LoadStartValues(void)
{
    u32 offset;
    u32 entry;
    u32 count;
    u32 bytesPerEntry;
    u32 i;
    u32 value;
    WlRfConfig *rf = &WIFI_DRIVER_STATE->rf;

    /* Firmware 0x44-0x63: start values for 16 registers listed in the table. */
    value = 0;
    for (i = 0; i < 16; i++) {
        WifiFlash_ReadRadioBytes(0x44 + i * 2, 2, &value);
        WIFI_MAC_REGISTER(gWlFirmwareRegOffsets[i]) = value;
    }

    /*
     * RF serial control: bits per entry, with the flag in bit 7 moved to bit 8.
     * The u32 cast is needed to match (lsr, not asr).
     */
    entry = ((u32)rf->bitsPerEntry >> 7) << 8;
    entry |= rf->bitsPerEntry & 0x7f;
    WIFI_MAC_REGISTER(0x184) = entry;

    /* The RF chip's start entries follow at firmware 0xce. */
    offset = 0xce;
    /* (bits + 7) / 8 through the signed divide; the low word holds the quotient. */
    bytesPerEntry = (u32)func_arm7_03806d4c((rf->bitsPerEntry & 0x7f) + 7, 8);
    count = rf->entryCount;
    if (rf->type == 3) {
        /* Type 3: one byte per entry, sent with its index; one more byte after them. */
        WifiFlash_ReadRadioBytes(count + 0xce, 1, &rf->unk_08);
        for (i = 0; i < count; i++, offset++) {
            entry = 0;
            WifiFlash_ReadRadioBytes(offset, 1, &entry);
            entry |= 0x50000 + (i << 8);
            func_arm7_027c8b64(entry);
        }
    } else {
        entry = 0;
        while (count != 0) {
            WifiFlash_ReadRadioBytes(offset, bytesPerEntry, &entry);
            func_arm7_027c8b64(entry);
            if (rf->type == 2 && (entry >> 18) == 9) {
                rf->unk_0c = entry & ~0x7c00;
            }
            count--;
            offset += bytesPerEntry;
        }
    }
}
