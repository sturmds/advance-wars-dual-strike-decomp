#ifndef SDK_SVC_H
#define SDK_SVC_H

#include "types.h"

#define SHA1_CHUNK_BYTES 64
#define SHA1_HASH_BYTES 20

/* Results of the SHA-1 routines. */
#define SHA1_STATUS_GOOD 0
#define SHA1_STATUS_NULL_INPUT 1
#define SHA1_STATUS_TOO_LONG 1         /* more than 2^64 bits hashed */
#define SHA1_STATUS_ALREADY_FINAL 3                /* update after the hash was taken */

/* State of a running SHA-1 hash, 0x68 bytes. */
typedef struct SVCSHA1Context {
    u32 state[5];                      /* the five hash words */
    u32 bitsLow;                       /* bits hashed so far, low word */
    u32 bitsHigh;
    u32 used;                          /* bytes waiting in block */
    u8 block[SHA1_CHUNK_BYTES];
    u32 finished;                      /* set once the hash was padded and taken */
    u32 error;                         /* nonzero stops all further input */
} SVCSHA1Context;

int Sha1_ResetState(SVCSHA1Context *context);
int Sha1_FeedBytes(SVCSHA1Context *context, const u8 *data, u32 length);
int Sha1_FinishDigest(SVCSHA1Context *context, u8 *digest);

#endif
