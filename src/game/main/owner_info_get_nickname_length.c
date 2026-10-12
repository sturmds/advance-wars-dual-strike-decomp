#include "types.h"
#include "game/owner_info.h"

int OwnerInfo_GetNicknameLength(void) {
  return gOwnerInfo.nicknameLength & 255;
}
