#include "types.h"
#include "game/link_player_name.h"

void Mem_CopyUnits(const void* src, void* dst, u32 size, u32 unitBits);

/* Stores the name a child sent for its aid. */
void Wireless_SetPlayerName(u32 aid, const LinkPlayerName* name) {
  Mem_CopyUnits(name, &gLinkPlayerNames[aid], sizeof(LinkPlayerName), 8);
}
