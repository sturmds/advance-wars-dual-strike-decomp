#include "types.h"

typedef struct Unk_02088af4 Unk_02088af4;

struct Unk_02088af4 {
    u8 pad_00[0x2c];
    int unk_2c;
    int unk_30;
    u8 pad_34[0x18];
    u16 unk_4c;
    u8 pad_4e[0x4];
    u16 unk_52;
};

void func_02088af4(Unk_02088af4 *a0)
{
    a0->unk_4c = 0;
    a0->unk_52 = 26;
    a0->unk_2c = 224;
    a0->unk_30 = 88;
}
