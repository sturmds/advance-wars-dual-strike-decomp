#include "types.h"

typedef struct Unk_02290718 Unk_02290718;

struct Unk_02290718 {
    u8 pad_00[0xf];
    u8 unk_f;
};

extern Unk_02290718 data_02290718;

int func_02018e84(void)
{
    return (data_02290718.unk_f == 0) ? 1 : 0;
}
