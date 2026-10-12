#include "types.h"

typedef struct Unk_027ce024 Unk_027ce024;
typedef struct Unk_027ce024_a1 Unk_027ce024_a1;

struct Unk_027ce024 {
    u8 pad_00[0x10];
    u16 unk_10;
};

struct Unk_027ce024_a1 {
    u8 pad_00[0x2];
    u16 unk_2;
};

void func_arm7_027ca30c(int, void *, int);

void func_arm7_027ce024(Unk_027ce024 *a0, Unk_027ce024_a1 *a1)
{
    a1->unk_2 = 1;
    func_arm7_027ca30c(a0->unk_10, a1, 1);
}
