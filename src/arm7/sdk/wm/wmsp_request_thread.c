#include "types.h"
#include "sdk/os_arm7.h"
#include "sdk/wmsp.h"

/* Thread: runs each request the ARM9 sends, marking WMStatus busy while the
   handler runs. Setting WM_API_REQUEST_ACCEPTED in the request afterwards
   lets the ARM9 reuse the buffer. A NULL message ends the thread. */
void Wmsp_RequestThread(void* arg) {
  WmspWork* work = func_arm7_027c05e0();
  WmspRequest* request;
  u16 apiid;

  while (TRUE) {
    MsgQueue_PopOldest_Arm7(&work->requestQueue, (OSMessage*)&request,
                            OS_MESSAGE_BLOCK);
    if (request == NULL) {
      Thread_EndCurrent_Arm7();
      return;
    }
    apiid = request->apiid;
    if (apiid & WM_API_REQUEST_ACCEPTED) {
      apiid &= ~WM_API_REQUEST_ACCEPTED;
    }
    if (apiid < WMSP_API_COUNT) {
      work->buf->status->apiBusy = TRUE;
      work->buf->status->busyApiid = apiid;
      gWmspRequestHandlers[apiid](request);
      work->buf->status->apiBusy = FALSE;
    }
    request->apiid = apiid | WM_API_REQUEST_ACCEPTED;
  }
}
