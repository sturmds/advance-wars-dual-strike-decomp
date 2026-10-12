#include "types.h"
#include "sdk/spi_thread.h"
#include "sdk/tp.h"
#include "sdk/os_arm7.h"

void NVRAM_ExecuteProcess(SerialBusJob* entry); /* NVRAM job */
void func_arm7_03805aa8(SerialBusJob* entry);   /* microphone job */
void PM_ExecuteProcess(SerialBusJob* entry);    /* power chip job */

/* Body of the SPI thread: waits for a job and runs it on its device's handler. */
void SerialDev_WorkerLoop(void* arg) {
  OSMessage msg;
  SerialBusJob* entry;

  while (TRUE) {
    MsgQueue_PopOldest_Arm7(&data_arm7_03809ab0.msgQ, &msg, OS_MESSAGE_BLOCK);
    entry = (SerialBusJob*)msg;
    switch (entry->device) {
      case SERIAL_OWNER_TOUCH:
        TP_ExecuteProcess(entry);
        break;
      case SERIAL_OWNER_MICROPHONE:
        func_arm7_03805aa8(entry);
        break;
      case SERIAL_OWNER_POWER_CHIP:
        PM_ExecuteProcess(entry);
        break;
      case SERIAL_OWNER_FLASH:
        NVRAM_ExecuteProcess(entry);
        break;
    }
  }
}
