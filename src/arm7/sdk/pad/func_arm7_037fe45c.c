#include "types.h"
#include "sdk/pad.h"
#include "sdk/os_arm7.h"

/* The ARM7 OS alarm; its layout is not described yet. */
struct OSAlarm;

/* 0x82e ticks of the 33.5 MHz / 64 timer: about 4 ms. */
#define EXTRA_KEYS_POLL_TICKS 0x82e

static BOOL data_arm7_03807b38;
static struct OSAlarm data_arm7_03807b3c;

void func_arm7_037fe41c(void* arg);

/* Starts the periodic alarm that copies the X/Y buttons and the lid state
 * for the ARM9. Returns FALSE when the tick or alarm system is not running
 * or the alarm is already set. */
BOOL func_arm7_037fe45c(void) {
  if (!Tick_IsRunning_Arm7() || !Alarm_IsSetUp_Arm7()) {
    return FALSE;
  }
  if (data_arm7_03807b38) {
    return FALSE;
  }
  Alarm_Clear_Arm7(&data_arm7_03807b3c);
  Alarm_ArmRepeating_Arm7(&data_arm7_03807b3c,
                          Tick_Read64_Arm7() + EXTRA_KEYS_POLL_TICKS,
                          EXTRA_KEYS_POLL_TICKS, func_arm7_037fe41c, NULL);
  data_arm7_03807b38 = TRUE;
  return TRUE;
}
