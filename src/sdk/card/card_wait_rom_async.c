#include "types.h"

BOOL CARD_WaitBackupAsync(void);

/* Waits for the running card request; ROM and backup requests share it. */
BOOL CARD_WaitRomAsync(void)
{
    return CARD_WaitBackupAsync();
}
