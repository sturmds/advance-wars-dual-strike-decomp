#include "types.h"
#include "sdk/spi_thread.h"

BOOL func_arm7_037fc3ac(OSMessageQueue *queue, OSMessage *msg, int flags); /* OS_ReceiveMessage */
void TP_ExecuteProcess(SerialBusJob *entry);
void func_arm7_027d43dc(SerialBusJob *entry);   /* NVRAM job */
void func_arm7_03805aa8(SerialBusJob *entry);   /* microphone job */
void func_arm7_03804da4(SerialBusJob *entry);   /* power chip job */

/* Body of the SPI thread: waits for a job and runs it on its device's handler. */
void SerialDev_WorkerLoop(void *arg)
{
    OSMessage msg;
    SerialBusJob *entry;

    while (TRUE) {
        func_arm7_037fc3ac(&data_arm7_03809ab0.msgQ, &msg, OS_MESSAGE_BLOCK);
        entry = (SerialBusJob *)msg;
        switch (entry->device) {
        case SERIAL_OWNER_TOUCH:
            TP_ExecuteProcess(entry);
            break;
        case SERIAL_OWNER_MICROPHONE:
            func_arm7_03805aa8(entry);
            break;
        case SERIAL_OWNER_POWER_CHIP:
            func_arm7_03804da4(entry);
            break;
        case SERIAL_OWNER_FLASH:
            func_arm7_027d43dc(entry);
            break;
        }
    }
}
