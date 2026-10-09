#include "types.h"

u32 Save_LoadProfiles(u32 co);
int func_02001250(int exp);

int func_021037ac(u32 co)
{
    return func_02001250(Save_LoadProfiles(co));
}
