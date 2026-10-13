#include "types.h"
#include "sdk/os.h"

#define EXCEPTION_HANDLER_MAIN_RAM (*(vu32*)0x027ffd9c)
#define SYSTEM_DTCM_BASE 0x027c3000

extern u32 data_022a9b84;
void OSi_ExceptionHandler(void);
u32 func_0210dbec(void);

/*
 * Keeps a user exception handler found in 0x02600000-0x027fffff, and installs
 * the system handler unless that user handler is kept and the flag word
 * has bit 30 set.
 */
void OS_InitException(void) {
  u32 userHandler = EXCEPTION_HANDLER_MAIN_RAM;
  vu32* dtcm = (vu32*)SYSTEM_DTCM_BASE;

  if (userHandler >= 0x02600000 && userHandler < 0x02800000) {
    data_022a9b84 = userHandler;
  } else {
    data_022a9b84 = 0;
  }
  if (data_022a9b84 == 0 || (func_0210dbec() & 0x40000000) == 0) {
    EXCEPTION_HANDLER_MAIN_RAM = (u32)OSi_ExceptionHandler;
    dtcm[0xfdc / 4] = (u32)OSi_ExceptionHandler;
  }
  data_022a9b7c = 0;
}
