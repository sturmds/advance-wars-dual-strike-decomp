#include "sdk/wl.h"

void WL_WriteByte(void* addr, u32 value);
u8 WL_ReadByte(const void* addr);
u32 WifiFrame_PutRatesElement(u8* dest);
int func_arm7_027c8c00(void);

/* Broadcast address ff:ff:ff:ff:ff:ff. */

#define REG_W_TXBUF_TIM (*(volatile u16*)0x04808084)

// Builds the beacon frame in WIFI_PACKET_RAM (WlState.txFrame): beacon interval and
// capability, then the SSID (left out when WlState.flags bit 0 is set), rates,
// DS parameter, TIM and Nintendo elements. Keeps the element offsets in
// WlLink for the code that updates the beacon later.
void WifiBeacon_Build(void) {
  WlWork* work = WIFI_DRIVER_STATE;
  WlRamFrame* frame = (WlRamFrame*)work->state.txFrame;
  WlHead* head = WIFI_SESSION_HEAD_OF(work);
  WlLink* link = WIFI_LINK_VIEW_OF(work);
  WlBeaconBody* body;
  u8* p;
  u32 i;
  u32 word;
  u8* src;

  frame->status = 0;
  frame->unk_02 = 0;
  frame->unk_04 = 0;
  frame->unk_06 = 0;
  frame->rate = func_arm7_027c8c00();
  frame->frameControl = 0x80; /* beacon */
  frame->duration = 0;
  WifiMac_CopyAddressTriple(frame->addr1, gWlBroadcastAddress, head->macAddress,
                            head->macAddress);
  frame->sequence = 0;
  body = (WlBeaconBody*)frame->body;
  body->timestamp[0] = 0;
  body->timestamp[1] = 0;
  body->beaconInterval = link->beaconInterval;
  body->capability = link->flags_7c;
  p = body->elements;

  if (head->flags.bit0 == 0) {
    link->ssidOffset = p - (u8*)body;
    WL_WriteByte(p++, 0);
    WL_WriteByte(p++, link->ssidLength & 0xff);
    for (i = 0; i < link->ssidLength; i++) {
      WL_WriteByte(p, WL_ReadByte(link->ssid + i));
      p++;
    }
    WL_WriteByte(p + (-1 - i), i & 0xff); /* the length byte again */
  } else {
    link->ssidOffset = 0;
  }
  p += WifiFrame_PutRatesElement(p);

  /* DS parameter set: the channel */
  WL_WriteByte(p, 3);
  WL_WriteByte(p + 1, 1);
  WL_WriteByte(p + 2, link->channel & 0xff);

  /* TIM: DTIM count, DTIM period, bitmap control, two bitmap bytes */
  link->timOffset = (p + 3) - (u8*)body;
  REG_W_TXBUF_TIM = link->timOffset + 2;
  WL_WriteByte(p + 3, 5);
  WL_WriteByte(p + 4, 5);
  WL_WriteByte(p + 5, 0);
  WL_WriteByte(p + 6, link->unk_74 & 0xff);
  WL_WriteByte(p + 7, 0);
  WL_WriteByte(p + 8, 0);
  WL_WriteByte(p + 9, 0);

  /* Vendor element, Nintendo OUI 00:09:bf, then the game info */
  link->gameInfoOffset = (p + 10) - (u8*)body;
  link->unk_a2 = link->gameInfoOffset & 1;
  WL_WriteByte(p + 10, 0xdd);
  WL_WriteByte(p + 11, (link->gameInfoLength + 8) & 0xff);
  WL_WriteByte(p + 12, 0);
  WL_WriteByte(p + 13, 9);
  WL_WriteByte(p + 14, 0xbf);
  WL_WriteByte(p + 15, 0);
  p += 16;
  if (link->unk_0e == 1) {
    WL_WriteByte(p++, head->unk_20 & 0xff);
    WL_WriteByte(p++, (head->unk_20 >> 8) & 0xff);
  } else {
    WL_WriteByte(p++, 0xff);
    WL_WriteByte(p++, 0xff);
  }
  word = WIFI_SHARED_GAME_WORD;
  WL_WriteByte(p++, word & 0xff);
  WL_WriteByte(p++, (word >> 8) & 0xff);
  src = link->gameInfo;
  for (i = 0; i < link->gameInfoLength; i++) {
    WL_WriteByte(p, WL_ReadByte(src));
    p++;
    src++;
  }
  /* Odd offset: moves the game info one byte up inside its own buffer. */
  if (link->unk_a2 != 0) {
    src = link->gameInfo + link->gameInfoLength - 1;
    for (i = 0; i < link->gameInfoLength; i++, src--) {
      WL_WriteByte(src + 1, WL_ReadByte(src));
    }
  }
  if (WIFI_DRIVER_STATE->unk_638 & 4) {
    u16* footer = (u16*)((u32)(p + 3) & ~3);
    footer[0] = WIFI_GUARD_WORD_A;
    footer[1] = WIFI_GUARD_WORD_B;
  }
  link->unk_a4 = 0;
  frame->length = (p + WIFI_HEADER_PLUS_CHECKSUM) - (u8*)body;
}
