#include "sdk/fs.h"

extern BOOL gFSInitialized;

void func_02112cac(u32 dmaNo);

/* Sets up the file system once; later calls do nothing. dmaNo is the DMA
 * channel the ROM archive reads with. */
void FS_Init(u32 dmaNo)
{
    if (gFSInitialized) {
        return;
    }
    gFSInitialized = TRUE;
    func_02112cac(dmaNo);
}
