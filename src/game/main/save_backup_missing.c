#include "types.h"

extern u8 gHasBackupMemory;

/* Returns TRUE when there is no backup memory (gHasBackupMemory is not 1).
 * The slot argument is not used. */
u16 Save_NoBackupMemory(u16 slot) { return gHasBackupMemory != 1; }
