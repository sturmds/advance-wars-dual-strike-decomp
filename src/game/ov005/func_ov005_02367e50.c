#include "types.h"

typedef struct Unk_02367e50_0 Unk_02367e50_0;

struct Unk_02367e50_0 {
    u8 pad_00[0x108];
    int unk_108;
};

extern Unk_02367e50_0 *data_ov005_0236bbb0;

int func_ov005_02367e50(void)
{
    return (data_ov005_0236bbb0->unk_108 == 0) ? 1 : 0;
}
