#include "types.h"
#include "sdk/wmsp.h"

BOOL Wmsp_IsOwnMacAddress(const u8 *macAddress)
{
    WMStatus *status = data_arm7_027d7c44.buf->status;

    if (macAddress[0] == status->macAddress[0]
        && macAddress[1] == status->macAddress[1]
        && macAddress[2] == status->macAddress[2]
        && macAddress[3] == status->macAddress[3]
        && macAddress[4] == status->macAddress[4]
        && macAddress[5] == status->macAddress[5]) {
        return TRUE;
    }
    return FALSE;
}
