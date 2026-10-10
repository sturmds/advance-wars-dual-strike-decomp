#include "types.h"

extern u8 data_0213fe0c[];
extern u8 data_021400d8[];

u8 *func_020d96e0(int kind)
{
    switch (kind) {
    case 3:
    default:
        return data_0213fe0c;
    case 4:
        return data_021400d8;
    }
}
