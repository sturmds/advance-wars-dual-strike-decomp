#include "types.h"

extern u8 data_ov000_022caf24[];
extern u8 data_ov000_022cb4ac[];
extern u8 data_ov000_022cb700[];

void* data_ov000_022f65c0[5] = {&data_ov000_022caf24, &data_ov000_022caf24,
                                &data_ov000_022caf24, &data_ov000_022caf24,
                                &data_ov000_022caf24};
/* per language: a second set of packed OBJ tiles */
u8* data_ov000_022f65d4[10] = {
    (u8*)&data_ov000_022cb4ac, (u8*)&data_ov000_022cb4ac,
    (u8*)&data_ov000_022cb4ac, (u8*)&data_ov000_022cb4ac,
    (u8*)&data_ov000_022cb4ac, (u8*)&data_ov000_022cb700,
    (u8*)&data_ov000_022cb700, (u8*)&data_ov000_022cb700,
    (u8*)&data_ov000_022cb700, (u8*)&data_ov000_022cb700};
