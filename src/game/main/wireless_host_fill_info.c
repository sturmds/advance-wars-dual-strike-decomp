#include "types.h"
#include "game/wireless_host.h"

extern WirelessHostUser gWirelessHostUser;
extern WirelessHostGame data_022a7e88;
extern const char data_0216a5b8[];
extern const char data_0216a5d0[];
extern const char data_0216a604[];
extern const char data_0216a638[];
extern const char data_0216a648[];

u8 OwnerInfo_GetBirthMonth(void);
int OwnerInfo_GetNicknameLength(void);
void OwnerInfo_CopyNickname(void *buffer);

/* Fills the user record (birth month, nickname) and the game record the
 * wireless host offers. */
void WirelessHost_FillInfo(void)
{
    gWirelessHostUser.birthMonth = OwnerInfo_GetBirthMonth();
    gWirelessHostUser.nameLength = OwnerInfo_GetNicknameLength();
    OwnerInfo_CopyNickname(&gWirelessHostUser.name);
    gWirelessHostUser.unk_0_4 = 0;
    data_022a7e88.unk_00 = data_0216a5b8;
    data_022a7e88.unk_04 = data_0216a5d0;
    data_022a7e88.unk_08 = data_0216a604;
    data_022a7e88.unk_0c = data_0216a638;
    data_022a7e88.unk_10 = data_0216a648;
    data_022a7e88.id = WIRELESS_HOST_ID;
    data_022a7e88.count = 8;
}
