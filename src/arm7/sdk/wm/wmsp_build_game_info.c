#include "types.h"
#include "sdk/wm.h"
#include "sdk/mi.h"

void Wmsp_BuildGameInfo(WMGameInfo *info, const WMParentParam *param)
{
    int attribute;

    info->ggid = param->ggid;
    info->tgid = param->tgid;
    attribute = (param->entryFlag != 0 ? WM_ATTR_FLAG_ENTRY : 0)
        | (param->multiBootFlag != 0 ? WM_ATTR_FLAG_MB : 0)
        | (param->KS_Flag != 0 ? WIRELESS_BEACON_BIT_KEYSHARE : 0)
        | (param->CS_Flag != 0 ? WIRELESS_BEACON_BIT_STREAMING : 0);
    info->attribute = attribute;
    info->userGameInfoLength = param->userGameInfoLength;
    info->magicNumber = 1;
    info->parentMaxSize = param->parentMaxSize;
    info->childMaxSize = param->childMaxSize;
    if (info->userGameInfoLength != 0) {
        Mem_CopyBytes_Arm7(param->userGameInfo, info->userGameInfo, (info->userGameInfoLength + 1) & ~1);
    }
}
