#include "types.h"

typedef struct Unk_02365ad4_0 Unk_02365ad4_0;

struct Unk_02365ad4_0 {
    u8 pad_00[0x24];
    int unk_24;
};

extern Unk_02365ad4_0 *data_ov005_0236bbac;

void func_ov005_02365ad4(void)
{
    data_ov005_0236bbac->unk_24 = 1;
}
