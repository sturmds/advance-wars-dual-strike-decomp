#include "sdk/fs.h"

static BOOL gFSInitialized;

void FSi_InitRom(u32 dmaNo);

/* Sets up the file system once; later calls do nothing. dmaNo is the DMA
 * channel the ROM archive reads with. */
void FS_Init(u32 dmaNo) {
  if (gFSInitialized) {
    return;
  }
  gFSInitialized = TRUE;
  FSi_InitRom(dmaNo);
}
