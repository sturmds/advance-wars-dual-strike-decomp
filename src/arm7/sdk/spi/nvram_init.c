#include "types.h"
#include "sdk/nvram.h"

/* Clears the request word buffer; SPI_Init calls it. */
void NVRAM_Init(void)
{
    int i;

    for (i = 0; i < NVRAM_PXI_WORDS; i++) {
        nvramPxiWords[i] = 0;
    }
}
