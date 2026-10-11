#include "types.h"
#include "game/Unk_0215fe8c.h"

extern Unk_0215fe8c *data_0215fe8c;

u32 Link_IsActive(void)
{
    return data_0215fe8c->linkActive;
}
