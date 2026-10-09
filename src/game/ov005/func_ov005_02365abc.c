#include "types.h"

typedef struct Unk_02365abc_0 Unk_02365abc_0;

struct Unk_02365abc_0 {
    u8 pad_00[0x2c];
    int unk_2c;
};

extern Unk_02365abc_0 *data_ov005_0236bbac;

void func_ov005_02365abc(void)
{
    data_ov005_0236bbac->unk_2c = 1;
}
