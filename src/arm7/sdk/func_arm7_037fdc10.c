#include "types.h"
#include "sdk/os.h"

void func_arm7_037f8444(void);
int func_arm7_037fdb80(void);

void func_arm7_037fdc10(void)
{
    while (TRUE) {
        func_arm7_037fdb80();
        func_arm7_037f8444();
    }
}
