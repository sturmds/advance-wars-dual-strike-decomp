#include "types.h"

extern BOOL gFSInitialized_Arm7;     /* set once FS_Init has run */

void FileSys_StartCardAccess_Arm7(u32 dmaNo);

/* Sets up the file system once; later calls do nothing. dmaNo is the DMA
 * channel for cartridge reads (main passes -1: no DMA). */
void FileSys_SetUpOnce_Arm7(u32 dmaNo)
{
    if (gFSInitialized_Arm7) {
        return;
    }
    gFSInitialized_Arm7 = TRUE;
    FileSys_StartCardAccess_Arm7(dmaNo);
}
