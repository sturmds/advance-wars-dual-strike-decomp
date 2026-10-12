#ifndef SDK_USER_SETTINGS_H
#define SDK_USER_SETTINGS_H

#include "types.h"

/*
 * The console's user settings as the firmware flash stores them. The flash
 * holds two copies; the ARM7 startup takes the one with a good CRC and the
 * newer save counter and copies it to 0x027ffc80 for the ARM9.
 */
typedef struct UserSettings {
    u8 unk_00[6];
    u16 nickname[10];           /* UTF-16 */
    u16 nicknameLength;
    u16 comment[26];            /* UTF-16 */
    u16 commentLength;
    u8 unk_52[0x1e];
    u16 saveCount;              /* 0-0x7f, one more on each save, wraps */
    u16 crc;                    /* CalcCrc16(0xffff, ...) of the first 0x70 bytes */
} UserSettings;

#define USER_SETTINGS_CRC_SIZE 0x70
#define USER_SETTINGS_NICKNAME_MAX 10
#define USER_SETTINGS_COMMENT_MAX 26

#endif
