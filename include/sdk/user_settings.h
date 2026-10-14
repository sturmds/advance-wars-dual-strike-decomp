#ifndef SDK_USER_SETTINGS_H
#define SDK_USER_SETTINGS_H

#include "types.h"
#include "sdk/os_common.h"

/*
 * The console's user settings as the firmware flash stores them. The flash
 * holds two copies; the ARM7 startup takes the one with a good CRC and the
 * newer save counter and copies it to 0x027ffc80 for the ARM9.
 */
typedef struct UserSettings {
  u8 unk_00[6];
  u16 nickname[10]; /* UTF-16 */
  u8 nicknameLength;
  u8 pad_1b;
  u16 comment[26]; /* UTF-16 */
  u8 commentLength;
  u8 pad_51;
  u8 unk_52[0x1e];
  u16 saveCount; /* 0-0x7f, one more on each save, wraps */
  u16 crc;       /* CalcCrc16(0xffff, ...) of the first 0x70 bytes */
} UserSettings;

#define USER_SETTINGS_CRC_SIZE 0x70
/* The flash holds this many copies of the settings. */
#define USER_SETTINGS_COPIES 2
#define USER_SETTINGS_NICKNAME_MAX 10
#define USER_SETTINGS_COMMENT_MAX 26

/*
 * Where the ARM7 startup leaves what it read from the firmware flash: the
 * chosen UserSettings, then the wireless block right after it.
 */
typedef struct SharedWirelessInfo {
  u8 macAddress[MAC_ADDRESS_SIZE];
  u16 channels; /* from flash offset 0x3c, through PickWirelessChannels */
} SharedWirelessInfo;

#define SHARED_USER_SETTINGS ((UserSettings*)0x027ffc80)
#define SHARED_WIRELESS_INFO ((SharedWirelessInfo*)(SHARED_USER_SETTINGS + 1))

/* Firmware flash offsets */
#define FLASH_USER_SETTINGS_OFFSET 0x20 /* halfword: user settings offset / 8 */
#define FLASH_MAC_ADDRESS 0x36
#define FLASH_CHANNELS 0x3c

#endif
