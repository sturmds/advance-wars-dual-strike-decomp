#include "types.h"
#include "sdk/mi.h"
#include "sdk/wl.h"

/* Fills size bytes at dest with a 16-bit pattern (hand-written assembly). */
void func_arm7_038057d4(u32 arg);

static u8 data_arm7_0380a030;
static u32 data_arm7_0380a038;
static u32 data_arm7_0380a034;
static u8 data_arm7_0380a03c[0x78];
WlStation data_arm7_0380a0b4[WIFI_STATION_TABLE_ENTRIES];
u8 data_arm7_0380a274[WIFI_DRIVER_STACK_SIZE];
u8 data_arm7_0380a874[WIFI_DRIVER_WORK_SIZE];

/* Called once from the ARM7 main: resets this module's state, then starts the wireless library. */
void func_arm7_038058b0(u32 arg) {
  data_arm7_0380a038 = 0;
  data_arm7_0380a034 = arg;
  Mem_FillBytes_Arm7(data_arm7_0380a03c, 0, sizeof(data_arm7_0380a03c));
  func_arm7_038057d4(arg);
  data_arm7_0380a030 = 3;
}
