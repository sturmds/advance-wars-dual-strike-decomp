#include "types.h"

typedef struct Unk_020f5e44 Unk_020f5e44;

struct Unk_020f5e44 {
    u8 pad_00[0x4c];
    int unk_4c;
};

void Mem_Free(int);

void func_020f5e44(Unk_020f5e44 *a0)
{
    Mem_Free(a0->unk_4c);
}
