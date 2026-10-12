#include "types.h"

typedef struct Unk_027cde74_a1 Unk_027cde74_a1;

struct Unk_027cde74_a1 {
    u8 pad_00[0x2];
    u16 unk_2;
};

void WifiParam_StoreSsidMask(int, void *, int);

void func_arm7_027cde74(int a0, Unk_027cde74_a1 *a1)
{
    a1->unk_2 = 1;
    WifiParam_StoreSsidMask(a0 + 16, a1, 1);
}
