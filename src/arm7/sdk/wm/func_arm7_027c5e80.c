#include "sdk/wmsp.h"

void func_arm7_027c5e80(WmspRequest* request) {
  if (func_arm7_027c05e0()->buf->status->state == WM_STATE_MP_PARENT) {
    WMSP_SendMp(request->arg);
  }
}
