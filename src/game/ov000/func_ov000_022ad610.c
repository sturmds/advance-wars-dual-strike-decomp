#include "types.h"

typedef struct Unk_022ad610 Unk_022ad610;

struct Unk_022ad610 {
    u8 pad_00[0x35];
    u8 unk_35;
};

void func_ov000_022adad8(void);
int func_020b98bc(int);

int func_ov000_022ad610(Unk_022ad610 *a0)
{
    func_ov000_022adad8();
    a0->unk_35 = 1;
    return func_020b98bc(1);
}
