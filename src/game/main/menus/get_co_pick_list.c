#include "types.h"

/* The pick list, filled by BuildCoPickList. */
extern u8 gCoPickList[];

u8* GetCoPickList(void) { return gCoPickList; }
