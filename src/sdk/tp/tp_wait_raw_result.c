#include "sdk/tp.h"

/* Waits for the sample asked for by TP_RequestSamplingAsync and copies it
 * to result. Returns 0 on success and 1 when the request failed. */
u32 TP_WaitRawResult(TPData* result) {
  TP_WaitBusy(TOUCH_JOB_ONE_SAMPLE_BIT);
  if (tpState.err_flg & TOUCH_JOB_ONE_SAMPLE_BIT) {
    return 1;
  }
  *result = tpState.buf;
  return 0;
}
