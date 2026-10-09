#include "types.h"

typedef struct Unk_02365aec_0 Unk_02365aec_0;

struct Unk_02365aec_0 {
    u8 pad_00[0x28];
    int unk_28;
};

extern Unk_02365aec_0 *data_ov005_0236bbac;

void func_ov005_02365aec(void)
{
    data_ov005_0236bbac->unk_28 = 1;
}
