#include "types.h"

int func_0209658c(void);
void func_02094d94(void);
int func_02093ef4(int unit);
void func_0209316c(int unit);

extern u8 data_0229e5bf[][12];

/* End-actions step 2. func_0209658c picks an index (-1: none). If
 * func_02093ef4 finds nothing for it, the record's status in data_0229e5bf
 * becomes 0xfe, else func_0209316c takes the index. Returns 1 when it picked one.
 * Purposes are not known. Only the register holding -1 for the first compare
 * differs (r0 here, r1 in the original). */
BOOL func_02091e40(void)
{
    int unit = func_0209658c();

    if (unit == -1) {
        return 0;
    }
    func_02094d94();
    if (func_02093ef4(unit) != -1) {
        func_0209316c(unit);
        return 1;
    }
    data_0229e5bf[unit][0] = 0xfe;
    return 1;
}
