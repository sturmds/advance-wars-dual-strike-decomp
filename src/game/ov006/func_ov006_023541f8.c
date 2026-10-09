#include "types.h"

void func_ov006_0235421c(void);
int EventScript_Start(void *);
extern u8 data_ov006_0236b40c[];

int func_ov006_023541f8(void)
{
    func_ov006_0235421c();
    return EventScript_Start(data_ov006_0236b40c);
}
