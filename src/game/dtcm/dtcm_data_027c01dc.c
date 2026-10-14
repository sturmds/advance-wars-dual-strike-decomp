#include "types.h"
#include "game/obj_size.h"

u8 data_027c01dc = {
#include "data/dtcm/data_027c01dc.inc.c"
};
/* 0x18 bytes no code names, before gObjSizes */
u32 data_dtcm_027c01e0[] = {
#include "data/dtcm/data_dtcm_027c01e0.inc.c"
};
ObjSize gObjSizes[][4] = {
#include "data/dtcm/gObjSizes.inc.c"
};
