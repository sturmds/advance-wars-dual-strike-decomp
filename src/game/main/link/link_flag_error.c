#include "types.h"
#include "game/Unk_0215fe8c.h"

u32 func_0207bd80(void);
extern Unk_0215fe8c *data_0215fe8c;

void Link_FlagError(void)
{
    if (func_0207bd80() == 1) {
        data_0215fe8c->error = 1;
    }
}
