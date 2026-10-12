#include "sdk/svc.h"

/* Writes the HMAC-SHA1 of data under key to digest (20 bytes), as RFC 2104
 * defines it. Does nothing when a pointer or a length is 0. */
void Sha1_KeyedDigest(u8* digest, const void* data, u32 dataLength,
                      const u8* key, int keyLength) {
  u8 innerPad[SHA1_CHUNK_BYTES];
  u8 outerPad[SHA1_CHUNK_BYTES];
  u8 hashBuffer[SHA1_HASH_BYTES];
  SVCSHA1Context context;
  u8* hash = hashBuffer; /* needed to match: kept in a register */
  int i;

  if (digest == NULL || data == NULL || dataLength == 0 || key == NULL ||
      keyLength == 0) {
    return;
  }
  if (keyLength > SHA1_CHUNK_BYTES) {
    Sha1_ResetState(&context);
    Sha1_FeedBytes(&context, key, keyLength);
    Sha1_FinishDigest(&context, hash);
    key = hash;
    keyLength = SHA1_HASH_BYTES;
  }
  /* Inner and outer pads: the key XOR 0x36 and XOR 0x5c, padded to one block. */
  for (i = 0; i < keyLength; i++) {
    innerPad[i] = *key ^ 0x36;
    outerPad[i] = *key++ ^ 0x5c;
  }
  for (; i < SHA1_CHUNK_BYTES; i++) {
    innerPad[i] = 0x36;
    outerPad[i] = 0x5c;
  }

  Sha1_ResetState(&context);
  Sha1_FeedBytes(&context, innerPad, SHA1_CHUNK_BYTES);
  Sha1_FeedBytes(&context, data, dataLength);
  Sha1_FinishDigest(&context, hash);

  Sha1_ResetState(&context);
  Sha1_FeedBytes(&context, outerPad, SHA1_CHUNK_BYTES);
  Sha1_FeedBytes(&context, hash, SHA1_HASH_BYTES);
  Sha1_FinishDigest(&context, digest);
}
