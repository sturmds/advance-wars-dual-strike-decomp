#include "sdk/mic.h"
#include "sdk/os_arm7.h"

void Irq_WriteVectorEntries(u32 mask, void (*handler)(void));
void IrqNest_UnhookVector(void);
void IrqNest_HookVector(void);
void func_arm7_03805a6c(void);

/* Runs one queued microphone request on the SPI thread. */
void func_arm7_03805aa8(SerialBusJob *entry)
{
    int irqState;
    u16 sample;

    switch (entry->command) {
    case MIC_PXI_COMMAND_ONE_SAMPLE:
        irqState = Cpu_MaskIrq_Arm7();
        if (!func_arm7_03803e68(SERIAL_JOB_MICROPHONE)) {
            Cpu_RestoreIrqBit_Arm7(irqState);
            SPIi_ReturnResult(entry->command, MIC_PXI_RESULT_EXCLUSIVE);
            break;
        }
        func_arm7_03803e50(SERIAL_JOB_MICROPHONE);
        Cpu_RestoreIrqBit_Arm7(irqState);
        if ((entry->args[0] & MIC_TYPE_12BIT) == MIC_TYPE_12BIT) {
            sample = func_arm7_038060bc();
            if (entry->args[0] & MIC_TYPE_SIGNED) {
                sample ^= 0x8000;
            }
            MIC_SYSTEM_WORK->lastSample = sample;
            MIC_SYSTEM_WORK->lastAddress = &MIC_SYSTEM_WORK->lastSample;
        } else {
            sample = func_arm7_03806184();
            if (entry->args[0] & MIC_TYPE_SIGNED) {
                sample ^= 0x80;
            }
            MIC_SYSTEM_WORK->lastSample = sample;
            MIC_SYSTEM_WORK->lastAddress = &MIC_SYSTEM_WORK->lastSample;
        }
        SPIi_ReturnResult(entry->command, MIC_PXI_RESULT_SUCCESS);
        SPIi_ReleaseException(SERIAL_JOB_MICROPHONE);
        break;

    case MIC_PXI_COMMAND_AUTO_START:
        if (data_arm7_0380af74.status == MIC_STATUS_AUTO_START) {
            data_arm7_0380af74.lastSample = 0;
            data_arm7_0380af74.pendingSample = 0;
            irqState = Cpu_MaskIrq_Arm7();
            Irq_SetEnableBits_Arm7(IRQ_MASK_TIMER3);
            Irq_WriteVectorEntries(IRQ_MASK_TIMER3, func_arm7_03805a6c);
            IrqNest_HookVector();
            REG_TM3CNT_L = data_arm7_0380af74.timerReload;
            REG_TM3CNT_H = data_arm7_0380af74.timerControl | TIMER_ENABLE | TIMER_IRQ_ENABLE;
            Cpu_RestoreIrqBit_Arm7(irqState);
            SPIi_ReturnResult(entry->command, MIC_PXI_RESULT_SUCCESS);
            data_arm7_0380af74.status = MIC_STATUS_AUTO_SAMPLING;
        } else {
            SPIi_ReturnResult(entry->command, MIC_PXI_RESULT_ILLEGAL_STATUS);
        }
        break;

    case MIC_PXI_COMMAND_AUTO_STOP:
        if (data_arm7_0380af74.status == MIC_STATUS_AUTO_END || data_arm7_0380af74.status == MIC_STATUS_END_WAIT) {
            REG_TM3CNT_H &= ~TIMER_ENABLE;
            irqState = Cpu_MaskIrq_Arm7();
            Irq_WriteVectorEntries(IRQ_MASK_TIMER3, NULL);
            IrqNest_UnhookVector();
            Cpu_RestoreIrqBit_Arm7(irqState);
            if (data_arm7_0380af74.status == MIC_STATUS_AUTO_END) {
                SPIi_ReturnResult(MIC_PXI_COMMAND_AUTO_STOP, MIC_PXI_RESULT_SUCCESS);
            } else {
                SPIi_ReturnResult(MIC_PXI_NOTICE_BUFFER_FULL, MIC_PXI_RESULT_SUCCESS);
            }
            data_arm7_0380af74.status = MIC_STATUS_READY;
        } else if (data_arm7_0380af74.status == MIC_STATUS_AUTO_END) {
            SPIi_ReturnResult(MIC_PXI_COMMAND_AUTO_STOP, MIC_PXI_RESULT_ILLEGAL_STATUS);
        } else {
            SPIi_ReturnResult(MIC_PXI_NOTICE_BUFFER_FULL, MIC_PXI_RESULT_ILLEGAL_STATUS);
        }
        break;
    }
}
