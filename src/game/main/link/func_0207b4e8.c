#include "types.h"
#include "game/Unk_0215fe8c.h"

extern Unk_0215fe8c *data_0215fe8c;

int func_0207a27c(void);
void Link_SendCommand(int type);

/* Link state 2 to 3: sends command 12. Purpose of the command is not worked out. */
void func_0207b4e8(void)
{
    data_0215fe8c->replyTimer = 0;
    if (data_0215fe8c->linkState != 2) {
        return;
    }
    if (func_0207a27c() != 4) {
        return;
    }
    data_0215fe8c->unk_04 = 0;
    Link_SendCommand(12);
    data_0215fe8c->linkState = 3;
}
