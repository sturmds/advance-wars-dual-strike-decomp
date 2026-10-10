#include "types.h"

extern u8 data_02168218[];
extern u8 data_0216821c[];

u8 *func_020d9730(int kind)
{
    switch (kind) {
    case 3:
    default:
        return data_02168218;
    case 4:
        return data_0216821c;
    }
}
