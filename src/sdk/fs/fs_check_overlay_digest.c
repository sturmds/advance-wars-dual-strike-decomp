#include "sdk/fs.h"

extern u8* gOverlayDigestKey;
extern u32 gOverlayDigestKeySize;

void MI_CpuFill8(void* dest, u8 value, u32 size);
void Mem_CopyBytes(const void* src, void* dest, u32 size);
void Sha1_KeyedDigest(u8* digest, const void* data, u32 size, const u8* key,
                      u32 keySize);

/* Computes the keyed digest of size bytes at data and returns whether it
 * equals the 20 bytes at expected. */
BOOL Overlay_CheckKeyedHash(const u8* expected, const void* data, u32 size) {
  u32 digest[5];
  u8 key[64];
  u32 offset;
  const u32* word;

  MI_CpuFill8(digest, 0, sizeof(digest));
  Mem_CopyBytes(gOverlayDigestKey, key, gOverlayDigestKeySize);
  Sha1_KeyedDigest((u8*)digest, data, size, key, gOverlayDigestKeySize);
  word = digest;
  for (offset = 0; offset < sizeof(digest); offset += 4) {
    if (*word != *(const u32*)(expected + offset)) {
      break;
    }
    word++;
  }
  return offset == sizeof(digest);
}
