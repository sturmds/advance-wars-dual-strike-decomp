#include "types.h"

void ScreenShake_Stop(void);
void ScreenShake_Start(int script, int arg, int parent);

void func_ov001_02351d74(void) {
  ScreenShake_Stop();
  ScreenShake_Start(2, 0, 0);
}
