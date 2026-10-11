#include "types.h"

typedef struct Unk_0210de74 Unk_0210de74;

struct Unk_0210de74 {
    u16 unk_0;
    u16 unk_2;
    int unk_4;
    int unk_8;
    int unk_c;
    int unk_10;
};

void OS_InitMessageQueue(Unk_0210de74 *a0, int a1, int a2)
{
    a0->unk_0 = 0;
    a0->unk_2 = 0;
    a0->unk_4 = a1;
    a0->unk_8 = a2;
    a0->unk_c = 0;
    a0->unk_10 = 0;
}
