#include "types.h"
#include "sdk/mi.h"
#include "sdk/wl.h"

/* Clears WlState.unk_d8 (16 halfwords). */
void func_arm7_027cb9ac(void) {
  /* needed to match: the original stores the fill value to the stack and reloads it */
  volatile u16 zero = 0;

  Mem_FillHalfwords_Arm7(zero, WIFI_DRIVER_STATE->state.unk_d8,
                         sizeof(WIFI_DRIVER_STATE->state.unk_d8));
}
