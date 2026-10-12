#include "sdk/user_settings.h"

u16 CalcCrc16(u16 crc, const u16* data, u32 size);

/*
 * Picks which of the two user settings copies read from flash to use:
 * 1 for copies[0], 2 for copies[1], 0 if neither has a good CRC and a save
 * count below 0x80. When both are good, the second is used only if its count
 * is one past the first's (mod 0x80).
 */
int UserSettings_ChooseCopy(UserSettings* copies) {
  u16 i;
  u32 valid = 0;

  for (i = 0; i < 2; i++) {
    UserSettings* copy = &copies[i];
    u32 crc = CalcCrc16(0xffff, (const u16*)copy, USER_SETTINGS_CRC_SIZE);
    if (crc == copy->crc && copy->saveCount < 0x80) {
      valid |= 1 << i;
    }
  }

  switch (valid) {
    case 1:
    case 2:
      return valid;
    case 3:
      return (u8)((copies[0].saveCount + 1) & 0x7f) == copies[1].saveCount ? 2
                                                                           : 1;
    default:
      return 0;
  }
}
