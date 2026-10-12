#include "types.h"
#include "game/save_data.h"

extern SaveBuffer data_02298050;
extern u8 data_0229a050[];

/* Checks the save buffer: the magic, the marker byte against the buffer's last byte (0x55 and
 * 0xaa, one each way round), the byte sum and its inverse, and the version byte 29. Returns 0
 * when the buffer is good, 1 when not. */
int Save_CheckBuffer(void) {
  u8 sum;
  int i;
  u8* byte;

  if (data_02298050.magic != SAVE_MAGIC) {
    return 1;
  }
  if (data_02298050.unk_04 == 0x55) {
    if (data_0229a050[0xfff] != 0xaa) {
      return 1;
    }
  } else if (data_02298050.unk_04 == 0xaa) {
    if (data_0229a050[0xfff] != 0x55) {
      return 1;
    }
  } else {
    return 1;
  }
  sum = 0;
  byte = (u8*)&data_02298050;
  for (i = 0; i < 0x3000; i++, byte++) {
    sum += *byte;
  }
  if (data_02298050.checksum == sum) {
    if (data_02298050.checksumInv != (u8)~sum) {
      return 1;
    }
  } else {
    return 1;
  }
  if (data_02298050.unk_05 == 29) {
    return 0;
  }
  return 1;
}
