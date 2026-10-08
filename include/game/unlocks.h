#ifndef GAME_UNLOCKS_H
#define GAME_UNLOCKS_H

#include "types.h"

/* One bit per unlockable: COs, cleared maps, hard mode, sound room. */
extern u8 gUnlockFlags[];

/* Returns nonzero if unlock flag `id` is set. */
u8 Unlock_IsSet(u32 id);

#endif
