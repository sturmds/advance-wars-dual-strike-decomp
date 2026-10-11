#include "types.h"

typedef struct Unk_02101698_0 Unk_02101698_0;

struct Unk_02101698_0 {
    u8 pad_00[0x118];
    int unk_118;
};

extern Unk_02101698_0 *gNameEntry;

int func_02101698(void)
{
    return gNameEntry->unk_118;
}
