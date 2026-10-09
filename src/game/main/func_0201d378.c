#include "types.h"
#include "game/Unk_0201d320.h"

extern u8 data_022900b8;
extern Unk_0201d320 data_022977f4[];
BOOL func_0201d6a0(s16 index);

BOOL func_0201d378(s16 index) {
    if (data_022900b8 != 1) {
        return func_0201d6a0(index);
    }
    data_022977f4[index].unk_00++;
    return 1;
}
