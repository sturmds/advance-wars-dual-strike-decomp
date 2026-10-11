#include "types.h"

extern u8 gHasBackupMemory;

int func_0200c2a0(void);
int func_0200c1c0(int a0, int a1, int a2);
void func_0201fb3c(void);

/* Sets gHasBackupMemory to 1 when backup memory answers, else shows the message screen
 * (func_0201fb3c). The original passes an uninitialised local as the second argument of
 * func_0200c1c0; needed to match. */
void Save_DetectBackupMemory(void)
{
    int buffer;

    gHasBackupMemory = 0;
    if (func_0200c2a0()) {
        if (func_0200c1c0(0, buffer, 4)) {
            gHasBackupMemory = 1;
            return;
        }
    }
    func_0201fb3c();
}
