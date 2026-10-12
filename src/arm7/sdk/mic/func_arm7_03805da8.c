#include "sdk/mic.h"

int func_arm7_037fdb80(void);
void func_arm7_037fdb94(int state);
BOOL func_arm7_03805d04(u32 span);

/*
 * Collects one PXI word of a microphone request from the ARM9. On the last
 * word it checks the request and its parameters and either queues it with
 * the SPI manager or replies with an error at once.
 */
void func_arm7_03805da8(u32 data)
{
    int i;
    u16 command;
    u32 buffer;
    u32 size;

    if (data & MIC_PXI_START) {
        for (i = 0; i < MIC_PXI_WORDS; i++) {
            data_arm7_0380af74.command[i] = 0;
        }
    }
    data_arm7_0380af74.command[(data & MIC_PXI_INDEX_MASK) >> MIC_PXI_INDEX_SHIFT] = data;
    if (!(data & MIC_PXI_END)) {
        return;
    }

    command = (data_arm7_0380af74.command[0] & 0xff00) >> 8;
    switch (command) {
    case MIC_PXI_COMMAND_ONE_SAMPLE:
        if (!SPIi_SetEntry(SERIAL_JOB_MICROPHONE, command, 1, data_arm7_0380af74.command[0] & 0xff)) {
            SPIi_ReturnResult(command, MIC_PXI_RESULT_EXCLUSIVE);
        }
        MIC_SYSTEM_WORK->lastSample = 0;
        MIC_SYSTEM_WORK->lastAddress = NULL;
        break;

    case MIC_PXI_COMMAND_AUTO_START:
        if (data_arm7_0380af74.status != MIC_STATUS_READY) {
            SPIi_ReturnResult(command, MIC_PXI_RESULT_ILLEGAL_STATUS);
            break;
        }
        data_arm7_0380af74.type = data_arm7_0380af74.command[0] & 0xff;
        buffer = (data_arm7_0380af74.command[1] << 16) | data_arm7_0380af74.command[2];
        if (buffer < MIC_BUFFER_MIN || buffer >= MIC_BUFFER_END) {
            SPIi_ReturnResult(command, MIC_PXI_RESULT_INVALID_PARAMETER);
            break;
        }
        data_arm7_0380af74.buffer = (u8 *)buffer;
        size = (data_arm7_0380af74.command[3] << 16) | data_arm7_0380af74.command[4];
        if (buffer + size > MIC_BUFFER_END) {
            SPIi_ReturnResult(command, MIC_PXI_RESULT_INVALID_PARAMETER);
            break;
        }
        data_arm7_0380af74.size = size;
        if (!func_arm7_03805d04((data_arm7_0380af74.command[5] << 16) | data_arm7_0380af74.command[6])) {
            SPIi_ReturnResult(command, MIC_PXI_RESULT_INVALID_PARAMETER);
            break;
        }
        data_arm7_0380af74.offset = 0;
        data_arm7_0380af74.mode = data_arm7_0380af74.type & 7;
        if (!SPIi_SetEntry(SERIAL_JOB_MICROPHONE, command, 0)) {
            SPIi_ReturnResult(command, MIC_PXI_RESULT_EXCLUSIVE);
            break;
        }
        MIC_SYSTEM_WORK->lastSample = 0;
        MIC_SYSTEM_WORK->lastAddress = NULL;
        data_arm7_0380af74.status = MIC_STATUS_AUTO_START;
        break;

    case MIC_PXI_COMMAND_AUTO_STOP:
        if (data_arm7_0380af74.status != MIC_STATUS_AUTO_SAMPLING) {
            SPIi_ReturnResult(command, MIC_PXI_RESULT_ILLEGAL_STATUS);
            break;
        }
        if (!SPIi_SetEntry(SERIAL_JOB_MICROPHONE, command, 0)) {
            SPIi_ReturnResult(command, MIC_PXI_RESULT_EXCLUSIVE);
            break;
        }
        data_arm7_0380af74.status = MIC_STATUS_AUTO_END;
        REG_TM3CNT_H &= ~TIMER_ENABLE;
        break;

    case MIC_PXI_COMMAND_AUTO_ADJUST:
        if (data_arm7_0380af74.status != MIC_STATUS_AUTO_SAMPLING) {
            SPIi_ReturnResult(command, MIC_PXI_RESULT_ILLEGAL_STATUS);
            break;
        }
        if (!func_arm7_03805d04((data_arm7_0380af74.command[1] << 16) | data_arm7_0380af74.command[2])) {
            SPIi_ReturnResult(command, MIC_PXI_RESULT_INVALID_PARAMETER);
            break;
        }
        {
            int irqState = func_arm7_037fdb80();

            REG_TM3CNT_H &= ~TIMER_ENABLE;
            REG_TM3CNT_L = data_arm7_0380af74.timerReload;
            REG_TM3CNT_H = data_arm7_0380af74.timerControl | TIMER_ENABLE | TIMER_IRQ_ENABLE;
            func_arm7_037fdb94(irqState);
        }
        SPIi_ReturnResult(command, MIC_PXI_RESULT_SUCCESS);
        break;

    default:
        SPIi_ReturnResult(command, MIC_PXI_RESULT_INVALID_COMMAND);
        break;
    }
}
