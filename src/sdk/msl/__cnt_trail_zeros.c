#include "types.h"
#include "sdk/msl.h"

int __cnt_trail_zeros(u32 value) {
  u8* byte;
  int zeros;

  if (value != 0) {
    byte = (u8*)&value;
    zeros = 0;
    while (*byte == 0) {
      byte++;
      zeros += 8;
    }
    *byte = ~*byte;
    while (*byte & 1) {
      zeros++;
      *byte >>= 1;
    }
    return zeros;
  }
  return 32;
}
