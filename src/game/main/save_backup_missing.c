#include "types.h"

extern u8 data_0229b050;

/* Returns TRUE when there is no backup memory (data_0229b050 is not 1).
 * The slot argument is not used. */
u16 Save_NoBackupMemory(u16 slot)
{
    return data_0229b050 != 1;
}
