#include "sdk/wl.h"

void Wifi_SwitchMode(int mode); /* switches WlLink.mode */
void func_arm7_027c90ec(u16* dst,
                        const u16* src); /* copies a 6-byte MAC address */
void WifiMlme_RunAuthentication(void);

// Switches to mode 0x20 and sends an 802.11 authentication frame to the
// access point at request->peer. The reply echoes the address and algorithm
// and goes out when the response arrives or the timeout runs out.
int WifiCmd_StartAuthentication(WlPeerRequest* request,
                                WlAuthenticateReply* reply) {
  WlWork* work = WIFI_DRIVER_STATE;
  WlLink* link = WIFI_LINK_VIEW_OF(work);
  WlPeerJob* job = &WIFI_ACTIVE_JOB_OF(work)->peer;

  reply->status = 6;
  if (work->state.phase != 3 && work->state.phase != 2) {
    return 11;
  }
  if (link->mode < 0x20) {
    return 1;
  }
  if (request->peer[0] & 1) {
    return WIFI_ANSWER_OUT_OF_RANGE;
  }
  if (request->authAlgorithm > 1) {
    return WIFI_ANSWER_OUT_OF_RANGE;
  }
  if (request->timeoutMs > 2000) {
    return WIFI_ANSWER_OUT_OF_RANGE;
  }
  if (request->timeoutMs < 10) {
    return WIFI_ANSWER_OUT_OF_RANGE;
  }
  Wifi_SwitchMode(0x20);
  job->request = request;
  job->reply = (WlReply*)reply;
  job->state = 0x30;
  /* needed to match: the original reads the request back from the job */
  reply->algorithm = job->request->authAlgorithm;
  func_arm7_027c90ec(reply->mac, job->request->peer);
  WifiMlme_RunAuthentication();
  return WIFI_ANSWER_LATER;
}
