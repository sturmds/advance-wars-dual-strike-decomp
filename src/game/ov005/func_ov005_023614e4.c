#include "types.h"

typedef struct Unk_023614e4_0 Unk_023614e4_0;

struct Unk_023614e4_0 {
    u8 pad_00[0xc];
    int unk_c;
};

extern Unk_023614e4_0 *data_ov005_0236bba4;

void func_ov005_023614e4(void)
{
    data_ov005_0236bba4->unk_c = 1;
}
