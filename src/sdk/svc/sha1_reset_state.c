#include "sdk/svc.h"

int Sha1_ResetState(SVCSHA1Context *context)
{
    if (context == NULL) {
        return SHA1_STATUS_NULL_INPUT;
    }
    context->bitsLow = 0;
    context->bitsHigh = 0;
    context->used = 0;
    context->state[0] = 0x67452301;
    context->state[1] = 0xefcdab89;
    context->state[2] = 0x98badcfe;
    context->state[3] = 0x10325476;
    context->state[4] = 0xc3d2e1f0;
    context->finished = 0;
    context->error = 0;
    return SHA1_STATUS_GOOD;
}
