#ifndef GAME_UNLOCKS_H
#define GAME_UNLOCKS_H

#include "types.h"

/* One bit per CO, set once the CO is unlocked. Other unlocks (maps, hard
 * mode, sound room) are game flags, see GameFlag_IsSet. */
extern u8 gUnlockFlags[];

/* Returns nonzero if unlock flag `id` is set. */
u8 Unlock_IsSet(u32 id);

/* What a Battle Maps shop condition routine returns for one item. */
/* Game flags Campaign_ResetProgress clears: GAME_FLAG_CAMPAIGN_FIRST up to,
 * not including, GAME_FLAG_CAMPAIGN_END. */
#define GAME_FLAG_CAMPAIGN_FIRST 95
#define GAME_FLAG_CAMPAIGN_END 159

#define SHOP_ITEM_BOUGHT (-1)
#define SHOP_ITEM_LOCKED 0
#define SHOP_ITEM_FOR_SALE 1

BOOL GameFlag_IsSet(int id);
void GameFlag_Set(int id, int value);
void CoColor_Set(u32 coId, int value);
void Unlock_Set(u32 id, int value);
u8 CoColor_IsSet(u32 coId);
#endif
