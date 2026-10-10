#include "types.h"

extern u8 data_02168210[];
extern u8 data_02168214[];

u8 *func_020d9708(int kind)
{
    switch (kind) {
    case 3:
    default:
        return data_02168210;
    case 4:
        return data_02168214;
    }
}
