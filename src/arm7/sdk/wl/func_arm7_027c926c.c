#include "sdk/wl.h"
#include "sdk/mi.h"

int func_arm7_027c926c(u32 length, const u8* src) {
  WlParams* params = WIFI_SETTINGS_BLOCK_OF(WIFI_DRIVER_STATE);

  if (length > 128) {
    return 4;
  }
  Mem_CopyHalfwords_Arm7(src, params->unk_9c, length + 1);
  params->unk_a0 = length;
  return 0;
}
