#include "types.h"
#include "sdk/spi_thread.h"

int func_arm7_037fdb80(void);                    /* OS_DisableInterrupts */
void func_arm7_037fdb94(int state);              /* OS_RestoreInterrupts */
void Thread_WaitOnQueue_Arm7(OSThreadQueue *queue);
void func_arm7_03803e50(int device);             /* take the bus */

/* Takes the SPI bus for ARM7 code outside the SPI thread, sleeping until it
 * is free. SPI_Unlock with the same id gives it back. */
void SPI_Lock(u32 id)
{
    int intr;

    while (TRUE) {
        intr = func_arm7_037fdb80();
        if (!data_arm7_03809ab0.busLocked) {
            break;
        }
        func_arm7_037fdb94(intr);
        Thread_WaitOnQueue_Arm7(&data_arm7_03809ab0.waitQueue);
    }
    func_arm7_03803e50(SERIAL_OWNER_OTHER_ARM7);
    data_arm7_03809ab0.lockId = id;
    func_arm7_037fdb94(intr);
}
