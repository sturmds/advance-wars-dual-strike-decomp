#include "types.h"
#include "sdk/os.h"

void MIi_CpuCopy16(const void *src, void *dst, u32 size);

void OS_GetOwnerInfo(OSOwnerInfo *info)
{
    FirmwareUserSettings *src = FIRMWARE_SETTINGS_COPY;

    info->favoriteColor = src->favoriteColor;
    info->birthMonth = src->birthMonth;
    info->birthDay = src->birthDay;
    info->unk_03 = src->unk_04;
    info->nicknameLength = src->nicknameLength;
    info->commentLength = src->commentLength;
    MIi_CpuCopy16(src->nickname, info->nickname, sizeof(src->nickname));
    MIi_CpuCopy16(src->comment, info->comment, sizeof(src->comment));
}
