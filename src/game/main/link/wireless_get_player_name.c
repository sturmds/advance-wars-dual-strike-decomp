#include "types.h"
#include "game/link_player_name.h"

/* Returns the name stored for an aid. */
LinkPlayerName* Wireless_GetPlayerName(u32 aid) {
  return &gLinkPlayerNames[aid];
}
