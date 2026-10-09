#include "types.h"

void func_02094bfc(void);
int func_020940f4(int arg);
void func_0209316c(int index);

/* End-actions step 9. func_020940f4 returns -1 when it finds nothing, else an
 * index that func_0209316c takes (the other steps pass it the same way). What
 * either does is not known yet. */
BOOL func_02091904(void)
{
    int index;

    func_02094bfc();
    index = func_020940f4(1);
    if (index == -1) {
        return 0;
    }
    func_0209316c(index);
    return 1;
}
