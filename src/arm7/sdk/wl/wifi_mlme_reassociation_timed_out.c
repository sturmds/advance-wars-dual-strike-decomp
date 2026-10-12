#include "sdk/wl.h"

/* Alarm handler of WifiMlme_RunReassociation: no answer came within request->timeoutMs. */
void WifiMlme_ReassociationTimedOut(void* arg) {
  WlPeerJob* job = &WIFI_ACTIVE_JOB_OF(WIFI_DRIVER_STATE)->peer;

  job->reply->unk_04 = WIFI_ANSWER_NO_RESPONSE;
  job->state = 0x63;
  WifiTask_Schedule(2, 4);
}
