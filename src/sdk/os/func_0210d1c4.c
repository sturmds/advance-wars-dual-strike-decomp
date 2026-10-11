#include "types.h"

void func_0210cf78(void);
int OSi_DoLockByWord(int a, int b, int c, int d);

void func_0210d1c4(int a, int b, int c, int d)
{
    while (OSi_DoLockByWord(a, b, c, d) > 0) {
        func_0210cf78();
    }
}
