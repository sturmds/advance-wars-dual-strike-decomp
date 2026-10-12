#include "types.h"

typedef struct Unk_037ff810 Unk_037ff810;

struct Unk_037ff810 {
    u8 pad_00[0x1d];
    u8 unk_1d;
};

void func_arm7_037ff810(Unk_037ff810 *a0, int a1)
{
    a0->unk_1d = a1;
}
