#include "sdk/snd_hw.h"
#include "sdk/os_arm7.h"

int func_arm7_037fe63c(void);
void func_arm7_037fe5dc(int);
void PMi_ResetControl(int);

void SndHw_Disable(void) {
  func_arm7_037fe63c();
  func_arm7_037fe5dc(0x80);
  Cpu_BusyWaitCycles_Arm7(0x40000);
  PMi_ResetControl(1);
  AUDIO_SPEAKER_POWER_REG &= ~1;
}
