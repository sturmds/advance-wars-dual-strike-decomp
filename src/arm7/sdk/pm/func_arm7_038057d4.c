#include "types.h"
#include "sdk/wm.h"
#include "sdk/wl.h"

void Wmsp_Init(Unk_arm7_038057d4* params, Unk_arm7_038057d4_2* params2);

/* The driver thread's stack; the driver gets its top, which is also where
   the work area starts. Written as base + size because the original has two
   pool entries for 0x0380a874. */
extern u8 data_arm7_0380a274[WIFI_DRIVER_STACK_SIZE];
extern u8 data_arm7_0380a874[WIFI_DRIVER_WORK_SIZE];
extern WlStation data_arm7_0380a0b4[WIFI_STATION_TABLE_ENTRIES];

/* Fills in the wireless library's init parameters and starts it; arg comes from the ARM7 main. */
void func_arm7_038057d4(u32 arg) {
  Unk_arm7_038057d4 params;
  Unk_arm7_038057d4_2 params2;

  /* needed to match: the stores stay in the original's order */
  params.unk_00 = data_arm7_0380a274 + WIFI_DRIVER_STACK_SIZE;
  params.unk_04 = data_arm7_0380a874;
  params.unk_08 = WIFI_DRIVER_STACK_SIZE;
  params.unk_0c = 4;
  params.unk_20 = 0;
  params.unk_24 = 8;
  params.unk_28 = arg;
  params.unk_2c = data_arm7_0380a0b4;
  params.unk_30 = sizeof(data_arm7_0380a0b4);
  params2.unk_00 = 3;
  params.unk_1c = 0x40;
  params2.unk_08 = 3;
  params2.unk_18 = 4;
  params2.unk_10 = 5;
  params2.unk_04 = 7;
  params2.unk_14 = 8;
  params2.unk_0c = 9;
  Wmsp_Init(&params, &params2);
}
