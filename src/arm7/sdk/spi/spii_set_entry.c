#include "types.h"
#include "stdarg.h"
#include "sdk/spi_thread.h"

int func_arm7_037fdb80(void);                /* OS_DisableInterrupts */
void func_arm7_037fdb94(int state);          /* OS_RestoreInterrupts */
BOOL func_arm7_037fc44c(OSMessageQueue *queue, OSMessage msg, int flags); /* OS_SendMessage */

/* Fills the next job slot with a device, a request id and up to
 * SERIAL_JOB_ARGS u32 arguments, then queues it for the SPI thread.
 * FALSE when there are too many arguments or the queue is full. */
BOOL SPIi_SetEntry(u32 device, u32 process, u16 paramCount, ...)
{
    int intr;
    u32 index;
    va_list args;
    int i;

    if (paramCount > SERIAL_JOB_ARGS) {
        return FALSE;
    }

    intr = func_arm7_037fdb80();
    data_arm7_03809ab0.entry[data_arm7_03809ab0.entryIndex].device = device;
    data_arm7_03809ab0.entry[data_arm7_03809ab0.entryIndex].process = process;
    va_start(args, paramCount);
    for (i = 0; i < paramCount; i++) {
        data_arm7_03809ab0.entry[data_arm7_03809ab0.entryIndex].arg[i] = va_arg(args, u32);
    }
    va_end(args);
    index = data_arm7_03809ab0.entryIndex;
    data_arm7_03809ab0.entryIndex = (index + 1) & (SERIAL_JOB_QUEUE_DEPTH - 1);
    func_arm7_037fdb94(intr);

    return func_arm7_037fc44c(&data_arm7_03809ab0.msgQ, &data_arm7_03809ab0.entry[index], OS_MESSAGE_NOBLOCK);
}
