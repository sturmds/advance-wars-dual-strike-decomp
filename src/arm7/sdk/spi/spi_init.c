#include "types.h"
#include "sdk/pm.h"
#include "sdk/pxi.h"
#include "sdk/spi_thread.h"
#include "sdk/os_arm7.h"

void Touch_ResetDriver_Arm7(void);
void func_arm7_027d485c(void);    /* NVRAM init */
void func_arm7_0380607c(void);    /* microphone init */
void Power_ResetDriverState_Arm7(void);    /* power chip init */
void func_arm7_037fe0a0(void);    /* PXI init */
void func_arm7_037fe298(u32 tag, PXIFifoCallback callback);           /* PXI_SetFifoRecvCallback */
void func_arm7_037fded4(void *dest, u32 data, u32 size);             /* MI_CpuFill8 */

extern u16 spiInitialized;

/* Starts the SPI devices and the thread that runs their jobs; later calls
 * do nothing. */
void SPI_Init(u32 priority)
{
    int i;

    if (spiInitialized) {
        return;
    }
    spiInitialized = TRUE;

    data_arm7_03809ab0.busLocked = FALSE;
    data_arm7_03809ab0.lockDevice = SERIAL_OWNER_NOBODY;
    Touch_ResetDriver_Arm7();
    func_arm7_027d485c();
    func_arm7_0380607c();
    Power_ResetDriverState_Arm7();
    func_arm7_037fe0a0();
    func_arm7_037fe298(FIFO_TAG_TOUCH, SerialDev_OnFifoMessage);
    func_arm7_037fe298(PXI_FIFO_TAG_MIC, SerialDev_OnFifoMessage);
    func_arm7_037fe298(PXI_FIFO_TAG_PM, SerialDev_OnFifoMessage);
    func_arm7_037fe298(PXI_FIFO_TAG_NVRAM, SerialDev_OnFifoMessage);

    MsgQueue_SetUpRing_Arm7(&data_arm7_03809ab0.msgQ, data_arm7_03809ab0.msgArray, SERIAL_JOB_QUEUE_DEPTH);
    for (i = 0; i < SERIAL_JOB_QUEUE_DEPTH; i++) {
        func_arm7_037fded4(&data_arm7_03809ab0.entry[i], 0, sizeof(SerialBusJob));
    }
    data_arm7_03809ab0.entryIndex = 0;
    data_arm7_03809ab0.waitQueue.head = 0;

    Thread_Spawn_Arm7(&data_arm7_03809ab0.thread, SerialDev_WorkerLoop, NULL,
                         data_arm7_03809ab0.stack + SERIAL_THREAD_STACK_BYTES / 4, SERIAL_THREAD_STACK_BYTES,
                         priority);
    Thread_MakeReady_Arm7(&data_arm7_03809ab0.thread);
}
