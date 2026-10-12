#include "types.h"
#include "sdk/nvram.h"
#include "sdk/os_arm7.h"
#include "sdk/spi.h"

/* Runs one NVRAM job on the SPI thread and replies to the ARM9. Reads need
 * an idle flash; writes and erases also need the write enable latch. */
void NVRAM_ExecuteProcess(SerialBusJob *entry)
{
    int irqState;

    irqState = Cpu_MaskIrq_Arm7();
    if (!SPIi_CheckException(SERIAL_OWNER_FLASH)) {
        Cpu_RestoreIrqBit_Arm7(irqState);
        SPIi_ReturnResult(entry->process, SPI_PXI_RESULT_EXCLUSIVE);
        return;
    }
    SPIi_GetException(SERIAL_OWNER_FLASH);
    Cpu_RestoreIrqBit_Arm7(irqState);

    switch (entry->process) {
    case FLASH_REQ_UNLOCK_WRITES:
        NVRAM_WriteEnable();
        break;
    case FLASH_REQ_LOCK_WRITES:
        NVRAM_WriteDisable();
        break;
    case FLASH_REQ_STATUS_BYTE:
        NVRAM_ReadStatus((u8 *)entry->arg[2]);
        break;
    case FLASH_REQ_LOAD:
        if (!NVRAM_IsIdle()) {
            SPIi_ReturnResult(entry->process, SPI_PXI_RESULT_ILLEGAL_STATUS);
            SPIi_ReleaseException(SERIAL_OWNER_FLASH);
            return;
        }
        NVRAM_Read(entry->arg[0], entry->arg[1], (u8 *)entry->arg[2]);
        break;
    case FLASH_REQ_LOAD_QUICK:
        if (!NVRAM_IsIdle()) {
            SPIi_ReturnResult(entry->process, SPI_PXI_RESULT_ILLEGAL_STATUS);
            SPIi_ReleaseException(SERIAL_OWNER_FLASH);
            return;
        }
        NVRAM_FastRead(entry->arg[0], entry->arg[1], (u8 *)entry->arg[2]);
        break;
    case FLASH_REQ_REWRITE_PAGE:
        if (!NVRAM_IsWriteEnabled()) {
            SPIi_ReturnResult(entry->process, SPI_PXI_RESULT_ILLEGAL_STATUS);
            SPIi_ReleaseException(SERIAL_OWNER_FLASH);
            return;
        }
        NVRAM_WritePage(entry->arg[0], entry->arg[1], (const u8 *)entry->arg[2]);
        break;
    case FLASH_REQ_PROGRAM_PAGE:
        if (!NVRAM_IsWriteEnabled()) {
            SPIi_ReturnResult(entry->process, SPI_PXI_RESULT_ILLEGAL_STATUS);
            SPIi_ReleaseException(SERIAL_OWNER_FLASH);
            return;
        }
        NVRAM_ProgramPage(entry->arg[0], entry->arg[1], (const u8 *)entry->arg[2]);
        break;
    case FLASH_REQ_CLEAR_PAGE:
        if (!NVRAM_IsWriteEnabled()) {
            SPIi_ReturnResult(entry->process, SPI_PXI_RESULT_ILLEGAL_STATUS);
            SPIi_ReleaseException(SERIAL_OWNER_FLASH);
            return;
        }
        NVRAM_ErasePage(entry->arg[0]);
        break;
    case FLASH_REQ_CLEAR_SECTOR:
        if (!NVRAM_IsWriteEnabled()) {
            SPIi_ReturnResult(entry->process, SPI_PXI_RESULT_ILLEGAL_STATUS);
            SPIi_ReleaseException(SERIAL_OWNER_FLASH);
            return;
        }
        NVRAM_EraseSector(entry->arg[0]);
        break;
    case FLASH_REQ_SLEEP:
        NVRAM_DeepPowerDown();
        break;
    case FLASH_REQ_WAKE:
        NVRAM_ReleasePowerDown();
        break;
    case FLASH_REQ_CLEAR_ALL:
        if (!NVRAM_IsWriteEnabled()) {
            SPIi_ReturnResult(entry->process, SPI_PXI_RESULT_ILLEGAL_STATUS);
            SPIi_ReleaseException(SERIAL_OWNER_FLASH);
            return;
        }
        NVRAM_ChipErase();
        break;
    case FLASH_REQ_IDENTIFY:
        NVRAM_ReadId((u8 *)entry->arg[2]);
        break;
    case FLASH_REQ_RESTART:
        NVRAM_SoftwareReset();
        break;
    default:
        SPIi_ReleaseException(SERIAL_OWNER_FLASH);
        SPIi_ReturnResult(entry->process, SPI_PXI_RESULT_INVALID_COMMAND);
        return;
    }
    SPIi_ReturnResult(entry->process, SPI_PXI_RESULT_SUCCESS);
    SPIi_ReleaseException(SERIAL_OWNER_FLASH);
}
