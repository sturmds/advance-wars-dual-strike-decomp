#include "sdk/wl.h"

int func_arm7_027c94ac(int value); /* writes WIFI_MAC_REGISTER(0x040) */
int func_arm7_027c94c0(int value); /* writes WIFI_MAC_REGISTER(0x03c) */
int func_arm7_027c94ec(int value); /* sets WlState.unk_26 */

int func_arm7_027cd2b4(WlRequest* request, WlReply* reply) {
  reply->status = 9;
  if (request->unk_10 > 1) {
    return WIFI_ANSWER_OUT_OF_RANGE;
  }
  if (request->unk_12 > 1) {
    return WIFI_ANSWER_OUT_OF_RANGE;
  }
  if (request->unk_14 > 1) {
    return WIFI_ANSWER_OUT_OF_RANGE;
  }
  func_arm7_027c94ec(request->unk_10);
  if (request->unk_10 == 1) {
    if (request->unk_12 == 1) {
      func_arm7_027c94ac(0x8001);
    } else {
      func_arm7_027c94ac(0);
    }
    WIFI_DRIVER_STATE->state.unk_2c = request->unk_14;
  } else {
    func_arm7_027c94ac(0x8000);
    func_arm7_027c94c0(2);
  }
  return WIFI_ANSWER_OK;
}
