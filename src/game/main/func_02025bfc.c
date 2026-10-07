#include "types.h"
#include "game/Unk_02025bfc.h"

extern Unk_02025bfc data_0229b3b0[80];

int func_02026404(Unk_02025bfc *entry, int mask);

void func_02025bfc(void)
{
    int i;
    Unk_02025bfc *entry;

    entry = data_0229b3b0;
    for (i = 0; i < 80; i++, entry++) {
        if (func_02026404(entry, 1) != 1) {
            entry->unk_01 = 0;
        }
    }
}
