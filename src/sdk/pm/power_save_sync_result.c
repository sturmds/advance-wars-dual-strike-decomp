#include "types.h"
#include "sdk/pm.h"

/* Callback of the synchronous PM calls: stores the command's result where
 * the caller waits for it. */
void Power_SaveSyncResult(u32 result, u32* dest) { *dest = result; }
