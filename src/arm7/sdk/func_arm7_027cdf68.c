#include "types.h"

typedef struct Unk_027cdf68 Unk_027cdf68;
typedef struct Unk_027cdf68_a1 Unk_027cdf68_a1;

struct Unk_027cdf68 {
    u8 pad_00[0x10];
    u16 unk_10;
};

struct Unk_027cdf68_a1 {
    u8 pad_00[0x2];
    u16 unk_2;
};

void WifiParam_StoreRateMode(int, void *, int);

void func_arm7_027cdf68(Unk_027cdf68 *a0, Unk_027cdf68_a1 *a1)
{
    a1->unk_2 = 1;
    WifiParam_StoreRateMode(a0->unk_10, a1, 1);
}
