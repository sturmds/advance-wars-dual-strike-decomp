#ifndef GAME_WIRELESS_HOST_H
#define GAME_WIRELESS_HOST_H

#include "types.h"
#include "game/link_player_name.h"

#define WIRELESS_HOST_ID 0x00400051

/* This console's owner, as the wireless host setup (WirelessHost_FillInfo) fills it:
 * birth month, nickname length and nickname. gWirelessHostUser (main bss). */
typedef struct WirelessHostUser {
  u8 birthMonth : 4;
  u8 unk_0_4 : 4; /* cleared */
  u8 nameLength;
  LinkPlayerName name;
  u16 pad_16;
} WirelessHostUser;

/* What the host offers to other consoles: six pointers (strings in main
 * data, purposes unknown), a 32-bit id and a count of 8.
 * data_022a7e88 (main bss). */
typedef struct WirelessHostGame {
  const char* unk_00; /* data_0216a5b8, also the file func_020fc89c opens */
  const char* unk_04;
  const char* unk_08;
  const char* unk_0c;
  const char* unk_10;
  u32 id;   /* WIRELESS_HOST_ID */
  u8 count; /* 8 */
} WirelessHostGame;

extern WirelessHostGame data_022a7e88;

#endif
