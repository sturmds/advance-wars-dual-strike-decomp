#include "types.h"
#include "game/owner_info.h"

/* Copies the owner's nickname to dest, zero-padded to 10 characters. */
void OwnerInfo_CopyNickname(u16* dest) {
  int i = 0;

  while (i < gOwnerInfo.nicknameLength) {
    dest[i] = gOwnerInfo.nickname[i];
    i++;
  }
  while (i < OWNER_NICKNAME_MAX) {
    dest[i] = 0;
    i++;
  }
}
