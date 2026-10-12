#include "types.h"
#include "sdk/wl.h"

void Alarm_ArmOneShot_Arm7(OSAlarm* alarm, OSTick delay, OSAlarmHandler handler,
                           void* arg);

/* Busy-waits usec microseconds on the one-shot alarm. handler gets a pointer to a flag and must clear it to end the wait. */
void Wifi_SpinForUs(u32 usec, OSAlarmHandler handler) {
  volatile u32 waiting = 1;

  Alarm_ArmOneShot_Arm7(&WIFI_DRIVER_STATE->oneShotAlarm,
                        Tick_FromMicroseconds32(usec), handler,
                        (void*)&waiting);
  while (waiting != 0) {
  }
}
