#include "types.h"
#include "sdk/pm.h"
#include "sdk/pxi.h"
#include "sdk/spi_thread.h"
#include "sdk/os_arm7.h"
#include "sdk/mi.h"
#include "sdk/tp.h"

void NVRAM_Init(void);                  /* NVRAM init */
void func_arm7_0380607c(void);          /* microphone init */
void Power_ResetDriverState_Arm7(void); /* power chip init */

static u16 spiInitialized;
SerialBusThreadState data_arm7_03809ab0;

/* Starts the SPI devices and the thread that runs their jobs; later calls
 * do nothing. */
void SPI_Init(u32 priority) {
  int i;

  if (spiInitialized) {
    return;
  }
  spiInitialized = TRUE;

  data_arm7_03809ab0.busLocked = FALSE;
  data_arm7_03809ab0.lockDevice = SERIAL_OWNER_NOBODY;
  Touch_ResetDriver_Arm7();
  NVRAM_Init();
  func_arm7_0380607c();
  Power_ResetDriverState_Arm7();
  Fifo_SetUp_Arm7();
  Fifo_RegisterTagHandler_Arm7(FIFO_TAG_TOUCH, SerialDev_OnFifoMessage);
  Fifo_RegisterTagHandler_Arm7(PXI_FIFO_TAG_MIC, SerialDev_OnFifoMessage);
  Fifo_RegisterTagHandler_Arm7(PXI_FIFO_TAG_PM, SerialDev_OnFifoMessage);
  Fifo_RegisterTagHandler_Arm7(PXI_FIFO_TAG_NVRAM, SerialDev_OnFifoMessage);

  MsgQueue_SetUpRing_Arm7(&data_arm7_03809ab0.msgQ, data_arm7_03809ab0.msgArray,
                          SERIAL_JOB_QUEUE_DEPTH);
  for (i = 0; i < SERIAL_JOB_QUEUE_DEPTH; i++) {
    Mem_FillBytes_Arm7(&data_arm7_03809ab0.entry[i], 0, sizeof(SerialBusJob));
  }
  data_arm7_03809ab0.entryIndex = 0;
  data_arm7_03809ab0.waitQueue.head = 0;

  Thread_Spawn_Arm7(&data_arm7_03809ab0.thread, SerialDev_WorkerLoop, NULL,
                    data_arm7_03809ab0.stack + SERIAL_THREAD_STACK_BYTES / 4,
                    SERIAL_THREAD_STACK_BYTES, priority);
  Thread_MakeReady_Arm7(&data_arm7_03809ab0.thread);
}
