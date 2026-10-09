#include "types.h"

typedef struct Unk_02290718 Unk_02290718;

struct Unk_02290718 {
    u8 pad_00[0x14];
    u8 unk_14;
};

extern Unk_02290718 data_02290718;
extern u8 data_ov005_0236a48c[];

u8 func_ov005_0235cc80(int a0)
{
    data_02290718.unk_14 = data_ov005_0236a48c[a0];
    return data_ov005_0236a48c[a0];
}
