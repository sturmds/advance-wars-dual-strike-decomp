#include "types.h"
#include "sdk/snd.h"

extern SndSharedWork* data_arm7_0380808c;

int func_arm7_03801e94(int a0, int a1) {
  SndSharedWork* t0 = data_arm7_0380808c;
  t0->globalVariable[a0] = a1;
  return ((int)t0 + (a0 << 1)) + 0x200;
}
