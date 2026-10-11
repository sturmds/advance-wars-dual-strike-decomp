#include "types.h"
#include "game/Unk_0215fe8c.h"

extern Unk_0215fe8c *data_0215fe8c;
extern u8 data_0229ca48[8];

int func_02078160(void *key, u16 *out);
u16 func_0207a1dc(void);

/* Runs each tick in link mode. While the link state is 2, the peer has to answer with the
 * local value | 1 (func_0207a1dc); more than 600 ticks without it flag an error. */
void Link_PollPeerReply(void)
{
    u16 reply;

    if (data_0215fe8c->unk_1c == 0) {
        return;
    }
    if (data_0215fe8c->unk_0b == 2) {
        if (func_02078160(data_0229ca48, &reply) && reply == (func_0207a1dc() | 1)) {
            data_0215fe8c->unk_14 = 1;
            data_0215fe8c->unk_00 = 0;
            return;
        }
        data_0215fe8c->unk_14 = 0;
        data_0215fe8c->unk_00++;
        if (data_0215fe8c->unk_00 > 600) {
            data_0215fe8c->unk_18 = 1;
        }
    } else {
        data_0215fe8c->unk_14 = 0;
        data_0215fe8c->unk_00 = 0;
    }
}
