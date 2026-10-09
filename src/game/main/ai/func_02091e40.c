#include "types.h"

int func_0209658c(void);
void func_02094d94(void);
int func_02093ef4(int unit);
void func_0209316c(int unit);

extern u8 data_0229e5bf[][12];

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
