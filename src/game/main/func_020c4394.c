#include "types.h"

typedef struct Unk_020c4394_a2 Unk_020c4394_a2;
typedef struct Unk_020c4394_a3 Unk_020c4394_a3;

struct Unk_020c4394_a2 {
    u16 unk_0;
    u16 unk_2;
};

struct Unk_020c4394_a3 {
    u16 unk_0;
    u16 unk_2;
};

void func_020c4394(int a0, int a1, Unk_020c4394_a2 *a2, Unk_020c4394_a3 *a3)
{
    a2->unk_0 = a0;
    a2->unk_2 = a1;
    a3->unk_0 = a0 << 4;
    a3->unk_2 = a1 << 4;
}
