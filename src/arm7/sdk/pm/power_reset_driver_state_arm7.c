#include "types.h"
#include "sdk/pm.h"

extern u32
    data_arm7_03809ff8; /* set once PM is ready; the ARM7 VBlank handler checks it */

/* Clears the power chip driver's state. Called from SPI_Init. */
void Power_ResetDriverState_Arm7(void) {
  int i;

  data_arm7_03809ff8 = TRUE;
  data_arm7_03809ffc.unk_20 = POWER_JOB_NONE;
  for (i = 0; i < POWER_MSG_MAX_HALFWORDS; i++) {
    data_arm7_03809ffc.words[i] = 0;
  }
}
