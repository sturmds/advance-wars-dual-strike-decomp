#ifndef GAME_OWNER_INFO_H
#define GAME_OWNER_INFO_H

#include "types.h"

/* The console owner's profile (favourite colour, birthday and nickname), laid
 * out like the SDK's OSOwnerInfo. Only the first 0x1a bytes are used so far. */
typedef struct OwnerInfo {
  u8 favoriteColor;
  u8 birthMonth;
  u8 birthDay;
  u8 pad_03;
  u16 nickname[10]; /* UTF-16 */
  u16 nicknameLength;
} OwnerInfo;

#define OWNER_NICKNAME_MAX 10

extern OwnerInfo gOwnerInfo;
/* gOwnerInfo's first byte, copied at boot; indexes the per-language graphics
 * tables (defined in ov000). */
extern u8 data_ov000_023503a0;

#endif
