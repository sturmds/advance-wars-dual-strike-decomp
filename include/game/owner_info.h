#ifndef GAME_OWNER_INFO_H
#define GAME_OWNER_INFO_H

#include "types.h"
#include "sdk/os_common.h"

/* The console owner's profile (favourite colour, birthday, nickname and
 * comment), as OS_GetOwnerInfo copies it at boot. */
typedef OSOwnerInfo OwnerInfo;

#define OWNER_NICKNAME_MAX 10

extern OwnerInfo gOwnerInfo;
/* gOwnerInfo's first byte, copied at boot; indexes the per-language graphics
 * tables (defined in ov000). */
extern u8 data_ov000_023503a0;

#endif
