#include "types.h"

void func_ov006_0235421c(void);
int EventScript_Start(void *);
extern u8 data_ov006_0236b2ac[];

int func_ov006_0235418c(void)
{
    func_ov006_0235421c();
    return EventScript_Start(data_ov006_0236b2ac);
}
