#include "types.h"
#include "game/unlocks.h"

u8 Unlock_IsSet(u32 id) { return (1 << (id & 7)) & gUnlockFlags[id >> 3]; }
