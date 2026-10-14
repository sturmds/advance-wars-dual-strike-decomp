#ifndef GAME_LINK_PLAYER_NAME_H
#define GAME_LINK_PLAYER_NAME_H

#include "types.h"

/* A player's nickname in link play: 10 characters, 20 bytes (the size
 * OwnerInfo_CopyNickname writes, as the stride of the table shows). */
#define LINK_PLAYER_NAME_LENGTH 10

typedef struct LinkPlayerName {
  u16 chars[LINK_PLAYER_NAME_LENGTH];
} LinkPlayerName;

/* gLinkPlayerNames (main bss): one name per aid. Slot 0 is this console. */
extern LinkPlayerName gLinkPlayerNames[];

#endif
