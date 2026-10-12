#include "types.h"
#include "game/link_player_name.h"

void Mem_ZeroUnits(u32 value, void* dst, u32 size, u32 unitBits);

/* Clears the name stored for an aid when its child leaves. */
void Wireless_ClearPlayerName(u32 aid) {
  Mem_ZeroUnits(0, &gLinkPlayerNames[aid], sizeof(LinkPlayerName), 8);
}
