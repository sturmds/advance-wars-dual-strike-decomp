#include "types.h"
#include "game/owner_info.h"

/* Copies the owner's nickname to dest, zero-padded to 10 characters. */
void OwnerInfo_CopyNickname(u16 *dest)
{
    int i = 0;

    while (i < data_0228ff58.nicknameLength) {
        dest[i] = data_0228ff58.nickname[i];
        i++;
    }
    while (i < OWNER_NICKNAME_MAX) {
        dest[i] = 0;
        i++;
    }
}
