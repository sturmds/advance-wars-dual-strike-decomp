#include "types.h"

void Font_SetDigitEndTile(int arg);
void *func_020176f4(const void *script, void *parent);
extern u8 data_ov006_0236b8b8[];

void func_ov006_02357920(void)
{
    Font_SetDigitEndTile(220);
    func_020176f4(data_ov006_0236b8b8, (void *)0);
}
