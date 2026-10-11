#include "types.h"

extern u8 data_0229b050;

int func_0200c2a0(void);
int func_0200c1c0(int a0, int a1, int a2);
void func_0201fb3c(void);

/* Sets data_0229b050 to 1 when backup memory answers, else shows the message screen
 * (func_0201fb3c). The original passes an uninitialised local as the second argument of
 * func_0200c1c0; needed to match. */
void Save_DetectBackupMemory(void)
{
    int buffer;

    data_0229b050 = 0;
    if (func_0200c2a0()) {
        if (func_0200c1c0(0, buffer, 4)) {
            data_0229b050 = 1;
            return;
        }
    }
    func_0201fb3c();
}
