#include "sdk/svc.h"

/* The four SHA-1 round constants. */
typedef struct Sha1RoundConstants {
  u32 k[4];
} Sha1RoundConstants;

static Sha1RoundConstants gSha1StageAdditives = {
#include "data/main/gSha1StageAdditives.inc.c"
};
static BOOL data_022ab8a4;

/* 80 rounds in four stages of 20, each with its own function and constant. */
#define SHA1_ROUNDS 80
#define SHA1_STAGE_ROUNDS (SHA1_ROUNDS / 4)

#define ROTL(bits, word) (((word) << (bits)) | ((word) >> (32 - (bits))))

/* Runs one 64-byte block through the SHA-1 compression function (80 rounds)
 * and adds the result to the intermediate hash. */
void Sha1_CompressBlock(SVCSHA1Context* context) {
  Sha1RoundConstants K = gSha1StageAdditives;
  u32 W[SHA1_ROUNDS];
  u32 A, B, C, D, E;
  u32 temp;
  int t;
  u32 saved[2];
  u32* blockWords = (u32*)context->block; /* declared last: needed to match */

  /* When set, block words 6 and 14 are hashed as zero and put back after. */
  if (data_022ab8a4) {
    saved[0] = blockWords[6];
    saved[1] = blockWords[14];
    blockWords[6] = 0;
    blockWords[14] = 0;
  }

  for (t = 0; t < SHA1_CHUNK_BYTES / 4; t++) {
    W[t] = context->block[t * 4] << 24;
    W[t] |= context->block[t * 4 + 1] << 16;
    W[t] |= context->block[t * 4 + 2] << 8;
    W[t] |= context->block[t * 4 + 3];
  }
  for (t = SHA1_CHUNK_BYTES / 4; t < SHA1_ROUNDS; t++) {
    W[t] = ROTL(1, W[t - 3] ^ W[t - 8] ^ W[t - 14] ^ W[t - 16]);
  }

  A = context->state[0];
  B = context->state[1];
  C = context->state[2];
  D = context->state[3];
  E = context->state[4];

  for (t = 0; t < SHA1_STAGE_ROUNDS; t++) {
    temp = ROTL(5, A) + ((B & C) | (~B & D)) + E + W[t] + K.k[0];
    E = D;
    D = C;
    C = ROTL(30, B);
    B = A;
    A = temp;
  }
  for (t = SHA1_STAGE_ROUNDS; t < 2 * SHA1_STAGE_ROUNDS; t++) {
    temp = ROTL(5, A) + (B ^ C ^ D) + E + W[t] + K.k[1];
    E = D;
    D = C;
    C = ROTL(30, B);
    B = A;
    A = temp;
  }
  for (t = 2 * SHA1_STAGE_ROUNDS; t < 3 * SHA1_STAGE_ROUNDS; t++) {
    temp = ROTL(5, A) + ((B & C) | (B & D) | (C & D)) + E + W[t] + K.k[2];
    E = D;
    D = C;
    C = ROTL(30, B);
    B = A;
    A = temp;
  }
  for (t = 3 * SHA1_STAGE_ROUNDS; t < SHA1_ROUNDS; t++) {
    temp = ROTL(5, A) + (B ^ C ^ D) + E + W[t] + K.k[3];
    E = D;
    D = C;
    C = ROTL(30, B);
    B = A;
    A = temp;
  }

  context->state[0] += A;
  context->state[1] += B;
  context->state[2] += C;
  context->state[3] += D;
  context->state[4] += E;
  context->used = 0;

  if (data_022ab8a4) {
    blockWords[6] = saved[0];
    blockWords[14] = saved[1];
  }
}
