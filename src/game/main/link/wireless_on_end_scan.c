#include "types.h"
#include "game/wireless_event.h"

extern u32 data_0229c880;

int func_02079180(void);
int func_02079368(void);
void Wireless_StartChannelSurvey(int arg);
BOOL Wireless_StartConnect(void);

/* Callback of the end-scan request in Wireless_EndScan: on success, runs the
 * next step for the current link state (0, or 3 to 5); state 5 starts
 * Wireless_StartConnect. */
void Wireless_OnEndScan(WirelessEvent* event) {
  if (event->errorCode == 0) {
    switch (data_0229c880) {
      case 0:
        func_02079368();
        return;
      case 3:
        func_02079180();
        return;
      case 4:
        Wireless_StartChannelSurvey(0);
        return;
      case 5:
        if (Wireless_StartConnect()) {
          return;
        }
        func_02079180();
        return;
    }
  } else {
    func_02079180();
  }
}
