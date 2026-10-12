#include "sdk/wl.h"

/* Pre-beacon interrupt. In mode 64 it queues task 13 once every unk_7e + 1
   pre-beacons, counting only while unk_72 equals listenInterval. */
void WifiIrq_BeforeBeacon(void) {
  WlLink* link = WIFI_LINK_VIEW_OF(WIFI_DRIVER_STATE);

  WIFI_IRQ_PENDING_FLAGS = W_IRQ_PRE_BEACON;
  if (link->mode == 64 && link->unk_7e != 0 &&
      link->unk_72 == link->listenInterval) {
    link->unk_80++;
    if (link->unk_80 > link->unk_7e) {
      link->unk_80 = 0;
      WifiTask_Schedule(1, 13);
    }
  }
  link->unk_10 = 1;
}
