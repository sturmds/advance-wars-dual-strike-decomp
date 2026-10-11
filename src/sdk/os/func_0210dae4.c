#include "types.h"
#include "sdk/os.h"

#define SHARED_FLAG_A (*(vu16 *)0x027ffc10)
#define SHARED_VALUE_A (*(vu16 *)0x027ff814)
#define SHARED_VALUE_B (*(vu16 *)0x027ffc14)

BOOL func_0210dae4(void)
{
    vu16 *value;

    if (SHARED_FLAG_A == 0) {
        value = &SHARED_VALUE_A;
    } else {
        value = &SHARED_VALUE_B;
    }
    return *value == 1;
}
