#include "sdk/wl.h"

void WL_WriteByte(void *addr, u32 value);
void func_arm7_027c84e0(void *dest, const void *src, u32 size); /* copies (size + 1) / 2 halfwords */

// Copies the current game info into the Nintendo element of the beacon frame
// in WIFI_PACKET_RAM (see WifiFrame_PutGameInfoElement), then fixes the element and frame
// lengths and clears WlLink.unk_a4.
void WifiBeacon_CopyGameInfo(void)
{
    WlLink *link = WIFI_LINK_VIEW_OF(WIFI_DRIVER_STATE);
    u8 *element = WIFI_DRIVER_STATE->state.txFrame->payload + link->gameInfoOffset;

    if (link->gameInfoLength != 0) {
        if (link->unk_a2 & 1) {
            // The game info starts one byte into the buffer: copy from the
            // buffer start onto byte 9, then put byte 9 back. Needed to
            // match: the address arithmetic on a u32.
            func_arm7_027c84e0((u8 *)((u32)(element + 10) - 1), link->gameInfo, link->gameInfoLength + 2);
            WL_WriteByte(element + 9, (WIFI_SHARED_GAME_WORD >> 8) & 0xff);
        } else {
            func_arm7_027c84e0(element + 10, link->gameInfo, link->gameInfoLength + 1);
        }
    }
    /* the 10 fixed bytes of the element come before the game info */
    ((WlRamFrame *)WIFI_DRIVER_STATE->state.txFrame)->length =
        link->gameInfoOffset + 10 + WIFI_HEADER_PLUS_CHECKSUM + link->gameInfoLength;
    WL_WriteByte(element + 1, (link->gameInfoLength + 8) & 0xff);
    if (WIFI_DRIVER_STATE->unk_638 & 4) {
        /* the same two halfwords WifiCmd_ArmMpReply puts after its frame body */
        u16 *footer = (u16 *)((u32)(element + 13 + link->gameInfoLength) & ~3);
        footer[0] = 0xb6b8;
        footer[1] = 0x1d46;
    }
    link->unk_a4 = 0;
}
