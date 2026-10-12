#include "types.h"

typedef struct Unk_03809ab0 Unk_03809ab0;

struct Unk_03809ab0 {
    int unk_0;
    int unk_4;
};

extern Unk_03809ab0 data_arm7_03809ab0;

void func_arm7_03803e50(int a0)
{
    data_arm7_03809ab0.unk_0 = 1;
    data_arm7_03809ab0.unk_4 = a0;
}
