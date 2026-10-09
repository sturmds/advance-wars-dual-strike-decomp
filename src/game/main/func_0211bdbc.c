#include "types.h"

typedef struct Unk_0211bdbc_0 Unk_0211bdbc_0;

struct Unk_0211bdbc_0 {
    u8 pad_00[0x50d];
    u8 unk_50d;
};

extern Unk_0211bdbc_0 *data_022aced8;

int func_0211bdbc(void)
{
    return (data_022aced8->unk_50d == 1) ? 1 : 0;
}
