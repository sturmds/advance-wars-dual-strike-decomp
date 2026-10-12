#ifndef GAME_WIRELESS_PARENT_INFO_H
#define GAME_WIRELESS_PARENT_INFO_H

#include "types.h"

/* What a parent advertises (data_0229c8a0 points to a 0x40 byte block of
 * this, filled at link init). The fields follow the arguments of the game
 * info request in Wireless_SetGameInfo. */
typedef struct WirelessParentInfo {
  void* userGameInfo;
  u16 userGameInfoLength;
  u8 pad_06[0x08 - 0x06];
  u32 ggid; /* game group id */
  u16 tgid;
} WirelessParentInfo;

extern WirelessParentInfo* data_0229c8a0;

#endif
