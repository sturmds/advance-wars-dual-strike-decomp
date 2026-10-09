#include "types.h"

typedef struct Unk_02290718 Unk_02290718;

struct Unk_02290718 {
    u8 pad_00[0x13];
    u8 unk_13;
};

extern Unk_02290718 data_02290718;

u8 func_ov005_02367e8c(void)
{
    return data_02290718.unk_13;
}
