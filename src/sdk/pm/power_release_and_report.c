#include "sdk/pm.h"

extern PowerCommandState gPMWork;

/* Releases the command lock and reports the result to the callback of the finished command. */
void Power_ReleaseAndReport(u32 result) {
  PMCallback callback = gPMWork.callback;
  u32* arg = gPMWork.arg;

  if (gPMWork.lock != 0) {
    gPMWork.lock = 0;
  }
  if (callback != NULL) {
    gPMWork.callback = NULL;
    callback(result, arg);
  }
}
