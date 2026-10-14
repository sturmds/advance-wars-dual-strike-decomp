#include "types.h"
#include "sdk/os_common.h"

void func_0210ccbc(void);
void func_0210cbd0(void);
void func_0210cbc0(void);
void func_0210cbb0(void);
void func_0210cba0(void);
void func_0210cc10(void);
void func_0210cc00(void);
void func_0210cbf0(void);
void func_0210cbe0(void);

#pragma explicit_zero_data on
u16 data_027c1278 = {
#include "data/dtcm/data_027c1278.inc.c"
};
OSIrqFunction OS_IRQTable[] = {&func_0210ccbc,
                               &func_0210ccbc,
                               &func_0210ccbc,
                               &func_0210cbd0,
                               &func_0210cbc0,
                               &func_0210cbb0,
                               &func_0210cba0,
                               &func_0210ccbc,
                               &func_0210cc10,
                               &func_0210cc00,
                               &func_0210cbf0,
                               &func_0210cbe0,
                               &func_0210ccbc,
                               &func_0210ccbc,
                               &func_0210ccbc,
                               &func_0210ccbc,
                               &func_0210ccbc,
                               &func_0210ccbc,
                               &func_0210ccbc,
                               &func_0210ccbc,
                               &func_0210ccbc,
                               &func_0210ccbc,
                               0,
                               0,
                               0};
#pragma explicit_zero_data reset
