#include "types.h"
#include "game/Unk_0215fe8c.h"

extern Unk_0215fe8c *data_0215fe8c;
extern u8 data_0229ca48[8];

int Wireless_StepDataSharing(void *key, u16 *out);
u16 Wireless_GetChildAids(void);

/* Runs each tick in link mode. While the link state is 2, the peer has to answer with the
 * local value | 1 (Wireless_GetChildAids); more than 600 ticks without it flag an error. */
void Link_PollPeerReply(void)
{
    u16 reply;

    if (data_0215fe8c->linkActive == 0) {
        return;
    }
    if (data_0215fe8c->linkState == 2) {
        if (Wireless_StepDataSharing(data_0229ca48, &reply) && reply == (Wireless_GetChildAids() | 1)) {
            data_0215fe8c->replyOk = 1;
            data_0215fe8c->replyTimer = 0;
            return;
        }
        data_0215fe8c->replyOk = 0;
        data_0215fe8c->replyTimer++;
        if (data_0215fe8c->replyTimer > 600) {
            data_0215fe8c->error = 1;
        }
    } else {
        data_0215fe8c->replyOk = 0;
        data_0215fe8c->replyTimer = 0;
    }
}
