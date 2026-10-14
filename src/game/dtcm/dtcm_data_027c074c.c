#include "types.h"
#include "game/range_map.h"

extern u32 data_027c0760[];

#pragma explicit_zero_data on
s8 data_027c074c = {
#include "data/dtcm/data_027c074c.inc.c"
};
MoveSearch* data_027c0758 = (MoveSearch*)&data_027c0760;
MoveSearchNode* data_027c0754 = 0;
int data_027c0750 = {
#include "data/dtcm/data_027c0750.inc.c"
};
MoveSearchNode* data_027c075c = 0;
u32 data_027c0760[] = {
#include "data/dtcm/data_027c0760.inc.c"
};
#pragma explicit_zero_data reset
