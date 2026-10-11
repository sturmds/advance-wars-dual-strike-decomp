#include "sdk/svc.h"

void func_021137d8(SVCSHA1Context *context);

/* Pads and finishes the hash the first time, then writes the 20 digest
 * bytes, most significant byte of each state word first. */
int Sha1_FinishDigest(SVCSHA1Context *context, u8 *digest)
{
    int i;

    if (context == NULL || digest == NULL) {
        return SHA1_STATUS_NULL_INPUT;
    }
    if (context->error != 0) {
        return context->error;
    }
    if (context->finished == 0) {
        func_021137d8(context);
        for (i = 0; i < SHA1_CHUNK_BYTES; i++) {
            context->block[i] = 0;
        }
        context->bitsLow = 0;
        context->bitsHigh = 0;
        context->finished = 1;
    }
    for (i = 0; i < SHA1_HASH_BYTES; i++) {
        digest[i] = context->state[i >> 2] >> ((3 - (i & 3)) * 8);
    }
    return SHA1_STATUS_GOOD;
}
