#include "types.h"
#include "sdk/mb.h"

void MIi_CpuCopy16(const void *src, void *dst, u32 size);

/* Stores the joined players' info. playerFlags has one bit per child (bit 1
 * up); the parent's bit 0 is added and the player count includes the parent. */
void MbGame_SetPlayers(DlPlayGameEntry *game, const void *users, u32 playerFlags, u32 unk)
{
    int i;
    u8 count = 1;

    MIi_CpuCopy16(users, game->users, sizeof(game->users));
    for (i = 0; i < DLPLAY_CHILD_SLOTS; i++) {
        if (playerFlags & (2 << i)) {
            count++;
        }
    }
    game->playerCount = count;
    game->playerFlags = playerFlags | 1;
    game->unk_35c = unk;
    game->changeCount++;
}
