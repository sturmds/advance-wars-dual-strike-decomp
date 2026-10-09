#include "types.h"

typedef struct Unk_0229da68 Unk_0229da68;

struct Unk_0229da68 {
    u8 pad_00[0x20];
    u8 unk_20;
    u8 unk_21;
};

extern Unk_0229da68 data_0229da68;
extern u16 data_0229da64;

void * func_0208109c(void)
{
    data_0229da68.unk_21 = 0;
    data_0229da68.unk_20 = data_0229da68.unk_21;
    data_0229da64 = 0;
    return &data_0229da64;
}
