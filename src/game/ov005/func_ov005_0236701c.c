#include "types.h"

typedef struct Unk_0236701c Unk_0236701c;
typedef struct Unk_0236701c_1 Unk_0236701c_1;

struct Unk_0236701c {
    u8 pad_00[0x34];
    int unk_34;
};

struct Unk_0236701c_1 {
    u8 pad_00[0x14];
    int unk_14;
};

extern Unk_0236701c_1 *data_ov005_0236bbb0;

void func_ov005_0236701c(Unk_0236701c *a0)
{
    data_ov005_0236bbb0->unk_14 = 2;
    a0->unk_34 = 0;
}
