#include "types.h"

typedef struct Unk_027cde5c Unk_027cde5c;
typedef struct Unk_027cde5c_a1 Unk_027cde5c_a1;

struct Unk_027cde5c {
    u8 pad_00[0x10];
    u16 unk_10;
};

struct Unk_027cde5c_a1 {
    u8 pad_00[0x2];
    u16 unk_2;
};

void WifiParam_SetShortPreamble(int, void *, int);

void func_arm7_027cde5c(Unk_027cde5c *a0, Unk_027cde5c_a1 *a1)
{
    a1->unk_2 = 1;
    WifiParam_SetShortPreamble(a0->unk_10, a1, 1);
}
