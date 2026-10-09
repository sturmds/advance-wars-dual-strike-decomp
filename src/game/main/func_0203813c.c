#include "types.h"

typedef struct Unk_0203813c_0 Unk_0203813c_0;

struct Unk_0203813c_0 {
    u8 pad_00[0x9a];
    u16 unk_9a;
};

extern Unk_0203813c_0 *data_0229c734;

void func_0203813c(int a0)
{
    data_0229c734->unk_9a = a0;
}
