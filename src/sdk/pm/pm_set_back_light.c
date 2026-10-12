#include "sdk/pm.h"

int PM_SetBackLightAsync(int target, int order,
                         void (*callback)(u32 result, u32* dest), u32* dest);

int PM_SetBackLight(int target, int order) {
  u32 result;
  int error;

  error = PM_SetBackLightAsync(target, order, Power_SaveSyncResult, &result);
  if (error != 0) {
    return error;
  }
  Power_SpinUntilIdle();
  return result;
}
