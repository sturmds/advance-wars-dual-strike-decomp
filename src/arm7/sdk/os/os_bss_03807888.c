#include "types.h"
#include "sdk/os_arm7.h"
#include "bss_run.h"

#pragma section SBSS begin
u16 data_arm7_03807888;
#pragma section SBSS end
#pragma section SBSS_RUN1 begin
u8 data_arm7_03807898[0x8]; /* 0x8 bytes no code names, before the end */
#pragma section SBSS_RUN1 end
#pragma section SBSS begin
/* count: as many elements as fit before the next symbol */
IrqSlotEntry data_arm7_0380788c[0x1];
#pragma section SBSS end
