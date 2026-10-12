#include "types.h"
#include "sdk/spi_thread.h"
#include "sdk/os_arm7.h"
#include "sdk/spi.h"

/* Takes the SPI bus for ARM7 code outside the SPI thread, sleeping until it
 * is free. SPI_Unlock with the same id gives it back. */
void SPI_Lock(u32 id)
{
    int intr;

    while (TRUE) {
        intr = Cpu_MaskIrq_Arm7();
        if (!data_arm7_03809ab0.busLocked) {
            break;
        }
        Cpu_RestoreIrqBit_Arm7(intr);
        Thread_WaitOnQueue_Arm7(&data_arm7_03809ab0.waitQueue);
    }
    SPIi_GetException(SERIAL_OWNER_OTHER_ARM7);
    data_arm7_03809ab0.lockId = id;
    Cpu_RestoreIrqBit_Arm7(intr);
}
