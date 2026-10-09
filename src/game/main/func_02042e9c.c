#include "types.h"

typedef struct Unk_02042e9c_0 Unk_02042e9c_0;

struct Unk_02042e9c_0 {
    u8 pad_00[0x31];
    u8 unk_31;
    u8 unk_32;
};

extern Unk_02042e9c_0 *data_0229c73c;
void func_0206cd64(int, int, int);

void func_02042e9c(int a0)
{
    Unk_02042e9c_0 *t0 = data_0229c73c;
    func_0206cd64(a0, t0->unk_31, t0->unk_32);
}
