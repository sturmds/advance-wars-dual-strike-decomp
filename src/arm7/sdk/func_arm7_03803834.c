#include "types.h"

typedef struct Unk_03803834_0 Unk_03803834_0;

struct Unk_03803834_0 {
    u8 pad_00[0xc];
    int unk_c;
    int unk_10;
    int unk_14;
};

void CardSave_ProgramPagesNoErase(int, int, int);

void func_arm7_03803834(Unk_03803834_0 **a0)
{
    Unk_03803834_0 *t0 = *a0;
    CardSave_ProgramPagesNoErase(t0->unk_10, t0->unk_c, t0->unk_14);
}
