#include "types.h"
#include "sdk/pm.h"
#include "sdk/spi.h"
#include "sdk/os_arm7.h"

void Power_SleepUntilTrigger(void);

extern u16 data_arm7_03809ff0; /* sleep: wake-up key pattern */
extern u16 data_arm7_03809ff4; /* sleep: wake-up triggers and backlights */

/* Runs one power chip job on the SPI thread, holding the bus meanwhile. */
void PM_ExecuteProcess(SerialBusJob* entry) {
  int intr = Cpu_MaskIrq_Arm7();

  if (!SPIi_CheckException(SERIAL_OWNER_POWER_CHIP)) {
    Cpu_RestoreIrqBit_Arm7(intr);
    SPIi_ReturnResult(entry->process, SPI_PXI_RESULT_EXCLUSIVE);
    return;
  }
  SPIi_GetException(SERIAL_OWNER_POWER_CHIP);
  Cpu_RestoreIrqBit_Arm7(intr);

  switch (entry->process) {
    case POWER_MSG_PREPARE_SLEEP:
      data_arm7_03809ffc.unk_20 = POWER_JOB_SLEEPING;
      data_arm7_03809ff4 = entry->arg[0];
      data_arm7_03809ff0 = entry->arg[1];
      Power_SleepUntilTrigger();
      break;
    case POWER_MSG_STORE_REGISTER: {
      u32 reg;
      u32 value;

      data_arm7_03809ffc.unk_20 = POWER_JOB_STORE;
      data_arm7_03809ffc.reg = reg = entry->arg[0];
      data_arm7_03809ffc.value = value = entry->arg[1];
      PMi_SetRegister((u16)reg, (u8)value); /* the original narrows both */
      SPIi_ReturnResult(POWER_MSG_STORE_REGISTER, SPI_PXI_RESULT_SUCCESS);
      break;
    }
    case POWER_MSG_FETCH_REGISTER: {
      u32 arg;
      u16 reg;

      data_arm7_03809ffc.unk_20 = POWER_JOB_FETCH;
      data_arm7_03809ffc.reg = arg = entry->arg[0];
      reg = arg;
      SPIi_ReturnResult(POWER_MSG_REGISTER_VALUE_BASE + reg,
                        PMi_GetRegister(reg));
      break;
    }
    case POWER_MSG_APPLY_ORDER: {
      u32 command;

      data_arm7_03809ffc.unk_20 = POWER_JOB_ORDER;
      data_arm7_03809ffc.value = command = entry->arg[0];
      Power_ApplyArm9Command(command);
      SPIi_ReturnResult(POWER_MSG_APPLY_ORDER, SPI_PXI_RESULT_SUCCESS);
      break;
    }
    case POWER_MSG_LED_STATE:
      PMi_SetLED(entry->arg[0]);
      break;
    default:
      SPIi_ReturnResult(entry->process, SPI_PXI_RESULT_INVALID_COMMAND);
      break;
  }

  SPIi_ReleaseException(SERIAL_OWNER_POWER_CHIP);
}
