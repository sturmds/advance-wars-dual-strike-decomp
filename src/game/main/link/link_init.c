#include "types.h"
#include "game/Unk_0215fe8c.h"

extern Unk_0215fe8c *data_0215fe8c;

void func_0207a888(void);
void func_020fcaf4(void);

/* Called once by main at start-up. */
void Link_Init(void)
{
    func_0207a888();
    func_020fcaf4();
    data_0215fe8c->unk_1c = 0;
}
