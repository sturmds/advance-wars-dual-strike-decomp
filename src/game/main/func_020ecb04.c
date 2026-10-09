#include "types.h"

typedef struct Unk_02081214 Unk_02081214;
struct Unk_02081214 {
    u8 pad_00[0x30];
    int unk_30;
};
void func_0202244c(int);

void func_020ecb04(Unk_02081214 *a0)
{
    func_0202244c(a0->unk_30);
}
