#include "types.h"

BOOL func_020240fc(int *value);

BOOL func_ov001_023516c8(void) {
    int value;

    if (!func_020240fc(&value)) {
        return 0;
    }
    if (value == 0) {
        return 1;
    }
    return 0;
}
