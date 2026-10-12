#include "types.h"
#include "game/wireless_event.h"

extern u32 data_0229c880;
extern u32 gWirelessState;

int func_02079180(void);
BOOL Wireless_StartMp(void);
BOOL Wireless_StartParentSetup(void);
BOOL Wireless_StartEventRequest(void);

/* Callback of the request in Wireless_StartRescan. On success: state 0
 * runs func_02079180; states 4 and 5 start MP communication; state 3 starts
 * the parent setup or the event request when gWirelessState is 4 or 5. A
 * failed start or an error code calls func_02079180; a failed MP start and
 * an error code also set state 3. */
void Wireless_OnRescan(WirelessEvent* event) {
  if (event->errorCode == 0) {
    switch (data_0229c880) {
      case 0:
        func_02079180();
        return;
      case 4:
      case 5:
        if (Wireless_StartMp()) {
          return;
        }
        data_0229c880 = 3;
        func_02079180();
        return;
      case 3:
        switch (gWirelessState) {
          case 4:
            if (Wireless_StartParentSetup()) {
              return;
            }
            func_02079180();
            return;
          case 5:
            if (Wireless_StartEventRequest()) {
              return;
            }
            func_02079180();
            return;
        }
        return;
    }
  } else {
    data_0229c880 = 3;
    func_02079180();
  }
}
