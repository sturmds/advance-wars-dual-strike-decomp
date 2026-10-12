#include "types.h"

typedef struct Unk_027cdf50 Unk_027cdf50;
typedef struct Unk_027cdf50_a1 Unk_027cdf50_a1;

struct Unk_027cdf50 {
    u8 pad_00[0x10];
    u16 unk_10;
};

struct Unk_027cdf50_a1 {
    u8 pad_00[0x2];
    u16 unk_2;
};

void func_arm7_027ca154(int, void *, int);

void func_arm7_027cdf50(Unk_027cdf50 *a0, Unk_027cdf50_a1 *a1)
{
    a1->unk_2 = 1;
    func_arm7_027ca154(a0->unk_10, a1, 1);
}
