#include "types.h"

typedef struct Unk_027cde8c Unk_027cde8c;
typedef struct Unk_027cde8c_a1 Unk_027cde8c_a1;

struct Unk_027cde8c {
    u8 pad_00[0x10];
    u16 unk_10;
};

struct Unk_027cde8c_a1 {
    u8 pad_00[0x2];
    u16 unk_2;
};

void func_arm7_027c9f64(int, int, int);

void func_arm7_027cde8c(Unk_027cde8c *a0, Unk_027cde8c_a1 *a1)
{
    a1->unk_2 = 1;
    func_arm7_027c9f64(a0->unk_10, 0, 1);
}
