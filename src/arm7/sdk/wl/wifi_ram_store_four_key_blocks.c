#include "sdk/wl.h"

void func_arm7_027c84e0(int dest, int src, int size);

int WifiRam_StoreFourKeyBlocks(int src) {
  func_arm7_027c84e0(0x04805f80, src, 20);
  func_arm7_027c84e0(0x04805fa0, src + 20, 20);
  func_arm7_027c84e0(0x04805fc0, src + 40, 20);
  func_arm7_027c84e0(0x04805fe0, src + 60, 20);
  return 0;
}
