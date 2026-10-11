#include "sdk/svc.h"

void func_02113924(SVCSHA1Context *context);

/* Pads the last block: a 0x80 byte, zeros, then the message length in bits
 * as a big-endian 64-bit number in the last 8 bytes. Takes two blocks when
 * the length does not fit after the 0x80 byte. The original compares used
 * as a signed number, hence the casts. */
void Sha1_AppendLengthPadding(SVCSHA1Context *context)
{
    if ((int)context->used > 55) {
        context->block[context->used++] = 0x80;
        while ((int)context->used < 64) {
            context->block[context->used++] = 0;
        }
        func_02113924(context);
        while ((int)context->used < 56) {
            context->block[context->used++] = 0;
        }
    } else {
        context->block[context->used++] = 0x80;
        while ((int)context->used < 56) {
            context->block[context->used++] = 0;
        }
    }
    context->block[56] = context->bitsHigh >> 24;
    context->block[57] = context->bitsHigh >> 16;
    context->block[58] = context->bitsHigh >> 8;
    context->block[59] = context->bitsHigh;
    context->block[60] = context->bitsLow >> 24;
    context->block[61] = context->bitsLow >> 16;
    context->block[62] = context->bitsLow >> 8;
    context->block[63] = context->bitsLow;
    func_02113924(context);
}
