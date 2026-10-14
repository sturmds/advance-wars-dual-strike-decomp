#include "types.h"

extern u32 data_0216d92c;

const char* gLanguageSuffix = (char*)&data_0216d92c; /* "_E" in this ROM */
u32 data_0216d93c = {
#include "data/main/data_0216d93c.inc.c"
};
u32 data_0216d938 = {
#include "data/main/data_0216d938.inc.c"
};
u32 data_0216d934 = {
#include "data/main/data_0216d934.inc.c"
};
u32 data_0216d930 = {
#include "data/main/data_0216d930.inc.c"
};
u32 data_0216d92c = {
#include "data/main/data_0216d92c.inc.c"
};
/* 0x10 bytes no code names, before gGxDispOn */
void* data_0216d944[] = {&data_0216d93c, &data_0216d938, &data_0216d934,
                         &data_0216d930};
