#include "types.h"
#include "sdk/spi_thread.h"
#include "sdk/os_arm7.h"

/* Gives back the bus that SPI_Lock took with the same id; other ids do nothing. */
void SPI_Unlock(u32 id)
{
    int intr;

    if (data_arm7_03809ab0.busLocked && data_arm7_03809ab0.lockDevice == SERIAL_OWNER_OTHER_ARM7
        && data_arm7_03809ab0.lockId == id) {
        intr = Cpu_MaskIrq_Arm7();
        data_arm7_03809ab0.lockDevice = SERIAL_OWNER_NOBODY;
        data_arm7_03809ab0.busLocked = FALSE;
        data_arm7_03809ab0.lockId = 0;
        Cpu_RestoreIrqBit_Arm7(intr);
        Thread_ReadyAllWaiters_Arm7(&data_arm7_03809ab0.waitQueue);
    }
}
