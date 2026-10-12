#include "sdk/wl.h"

void WL_WriteByte(void* addr, u32 value);
u8 WL_ReadByte(const void* addr);

// Writes the vendor element of Nintendo's beacons at dest (id 0xdd, OUI
// 00:09:bf, type 0, two halfwords, then the game info) and returns its size.
u32 WifiFrame_PutGameInfoElement(u8* dest) {
  // needed to match: this declaration order
  WlLink* link;
  u32 word;
  WlWork* work;
  WlStateView* view;
  const u8* src;
  u32 i;
  u32 length;

  work = WIFI_DRIVER_STATE;
  link = WIFI_LINK_VIEW_OF(work);
  view = WIFI_FROM_STATION_TABLE_OF(work);
  length = 0;

  WL_WriteByte(dest, 0xdd);
  WL_WriteByte(dest + 1, (WIFI_DRIVER_STATE->state.unk_b8 + 8) & 0xff);
  WL_WriteByte(dest + 2, length);
  WL_WriteByte(dest + 3, 0x09);
  WL_WriteByte(dest + 4, 0xbf);
  WL_WriteByte(dest + 5, length);
  WL_WriteByte(dest + 6, view->state.unk_10 & 0xff);
  WL_WriteByte(dest + 7, (view->state.unk_10 >> 8) & 0xff);
  word = WIFI_SHARED_GAME_WORD;
  WL_WriteByte(dest + 8, word & 0xff);
  WL_WriteByte(dest + 9, (word >> 8) & 0xff);
  length += 10;
  if (link->gameInfoLength != 0) {
    src = link->gameInfo;
    if (link->unk_a2 & 1) {
      src++;
    }
    for (i = 0; i < link->gameInfoLength; i++) {
      WL_WriteByte(dest + length, WL_ReadByte(src));
      length++;
      src++;
    }
  }
  return length;
}
