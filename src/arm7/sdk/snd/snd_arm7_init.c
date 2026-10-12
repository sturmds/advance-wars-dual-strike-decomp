#include "sdk/snd.h"
#include "sdk/os_arm7.h"

void func_arm7_0380239c(void);

void SndArm7_Init(void) {
  MsgQueue_SetUpRing_Arm7(&data_arm7_03809210, data_arm7_03809224, 8);
  func_arm7_0380239c();
  data_arm7_0380808c = NULL;
}
