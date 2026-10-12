#include "types.h"
#include "game/wireless_event.h"

extern u32 data_0229c880;
extern u32 gWirelessState;

void Wireless_RequestChildRole(void);
void Wireless_RequestParentRole(void);

/* Callback of the request in func_02079578. On success clears gWirelessState
 * and, for modes 3 to 5, runs the follow-up call for the mode; on an error
 * sets gWirelessState to 1. */
void Wireless_OnStepDone(WirelessEvent* event) {
  if (event->errorCode == 0) {
    switch (data_0229c880) {
      case 0:
        gWirelessState = 0;
        return;
      case 4:
        gWirelessState = 0;
        Wireless_RequestParentRole();
        return;
      case 3:
      case 5:
        gWirelessState = 0;
        Wireless_RequestChildRole();
        return;
    }
  } else {
    gWirelessState = 1;
  }
}
