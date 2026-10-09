#ifndef GAME_UNLOCKS_H
#define GAME_UNLOCKS_H

#include "types.h"

/* One bit per unlockable: COs, cleared maps, hard mode, sound room. */
extern u8 gUnlockFlags[];

/* Returns nonzero if unlock flag `id` is set. */
u8 Unlock_IsSet(u32 id);

/* What a Battle Maps shop condition routine returns for one item. */
#define SHOP_ITEM_BOUGHT   (-1)
#define SHOP_ITEM_LOCKED   0
#define SHOP_ITEM_FOR_SALE 1

BOOL GameFlag_IsSet(int id);
void GameFlag_Set(int id, int value);
#endif
