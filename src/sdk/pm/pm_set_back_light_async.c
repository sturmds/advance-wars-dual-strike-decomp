#include "sdk/pm.h"

int Power_PostCommandWithReply(int command,
                               void (*callback)(u32 result, u32* dest),
                               u32* dest);

/* target 0, 1, 2 select the commands 6/7, 4/5 and 8/9 (on/off); order is 1 for on, 0 for off. 0xffff: bad argument. */
int PM_SetBackLightAsync(int target, int order,
                         void (*callback)(u32 result, u32* dest), u32* dest) {
  int command = 0;

  if (target == 0) {
    if (order == 1) {
      command = 6;
    }
    if (order == 0) {
      command = 7;
    }
  } else if (target == 1) {
    if (order == 1) {
      command = 4;
    }
    if (order == 0) {
      command = 5;
    }
  } else if (target == 2) {
    if (order == 1) {
      command = 8;
    }
    if (order == 0) {
      command = 9;
    }
  }
  if (command == 0) {
    return 0xffff;
  }
  return Power_PostCommandWithReply(command, callback, dest);
}
