#include "sdk/svc.h"

void Sha1_CompressBlock(SVCSHA1Context *context);

/* Adds bytes to the hash; every full 64-byte block is processed at once. */
int Sha1_FeedBytes(SVCSHA1Context *context, const u8 *data, u32 length)
{
    if (length == 0) {
        return SHA1_STATUS_GOOD;
    }
    if (context == NULL || data == NULL) {
        return SHA1_STATUS_NULL_INPUT;
    }
    if (context->finished != 0) {
        context->error = SHA1_STATUS_ALREADY_FINAL;
        return SHA1_STATUS_ALREADY_FINAL;
    }
    if (context->error != 0) {
        return context->error;
    }
    while (length-- != 0 && context->error == 0) {
        context->block[context->used++] = *data;
        context->bitsLow += 8;
        if (context->bitsLow == 0) {
            context->bitsHigh++;
            if (context->bitsHigh == 0) {
                context->error = SHA1_STATUS_TOO_LONG;
            }
        }
        if (context->used == SHA1_CHUNK_BYTES) {
            Sha1_CompressBlock(context);
        }
        data++;
    }
    return SHA1_STATUS_GOOD;
}
