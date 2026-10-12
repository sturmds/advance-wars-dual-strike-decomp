#include "sdk/wl.h"

u8 WL_ReadByte(const void* addr);
void WifiMlme_RunChannelSurvey(void);

/*
 * Checks a measure-channel request and starts WifiMlme_RunChannelSurvey. Returns
 * 1 unless state.mode is 0x20, WIFI_ANSWER_OUT_OF_RANGE for an empty channel
 * list or a value out of range, and 0x80 once the measurement runs.
 */
int WifiCmd_StartChannelSurvey(WlMeasureChannelRequest* request,
                               WlReply* reply) {
  WlMeasureChannelJob* job = &WIFI_ACTIVE_JOB_OF(WIFI_DRIVER_STATE)->measure;
  u32 i;

  reply->status = 18;
  if (WIFI_DRIVER_STATE->state.mode != 0x20) {
    return 1;
  }
  if (request->reg13Value > 3) {
    return WIFI_ANSWER_OUT_OF_RANGE;
  }
  if (request->reg35Value > 0x3f) {
    return WIFI_ANSWER_OUT_OF_RANGE;
  }
  if (request->sampleMs == 0) {
    return WIFI_ANSWER_OUT_OF_RANGE;
  }
  if (request->sampleMs > 1000) {
    return WIFI_ANSWER_OUT_OF_RANGE;
  }
  for (i = 0; i < 16; i++) {
    u8 channel = WL_ReadByte(&request->channels[i]);
    if (channel == 0) {
      break;
    }
    if (WifiParam_ChannelAllowed(channel) == 0) {
      return WIFI_ANSWER_OUT_OF_RANGE;
    }
  }
  if (i == 0) {
    return WIFI_ANSWER_OUT_OF_RANGE;
  }
  job->request = request;
  job->reply = reply;
  job->state = 0x80;
  reply->unk_04 = 0x80;
  WifiMlme_RunChannelSurvey();
  return 0x80;
}
