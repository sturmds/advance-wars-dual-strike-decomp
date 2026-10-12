#include "sdk/wmsp.h"

/* As in sdk/mi.h, which clashes with wmsp.h over Mem_CopyHalfwords_Arm7. */

u16 *func_arm7_027c21f0(WlCommand *cmd);
void Wmsp_ReplyFailed(int apiid, int wlCmdID, int wlResult);

/* WL command 0x200: the station address, channels and the other WL settings
   in WMStatus, all in one command. */
BOOL WMSP_SetAllParams(int apiid, WlCommand *cmd)
{
    WmspBuf *buf = func_arm7_027c05e0()->buf;
    WlParamSetAllCommand *param = (WlParamSetAllCommand *)cmd;
    volatile u16 zero;
    volatile u16 zeroMask;
    volatile u16 fullMask;
    u16 *result;

    Mem_CopyBytes_Arm7(buf->status->macAddress, param->macAddress, sizeof(param->macAddress));
    param->unk_16 = 7;
    param->enableChannel = buf->status->enableChannel;
    param->unk_1e = buf->status->unk_1e8;
    param->unk_1c = buf->status->unk_e2;
    if (buf->status->wepFlag == 0) {
        param->unk_20 = 0;
        param->unk_22 = 0;
        zero = 0;
        Mem_FillHalfwords_Arm7(zero, param->unk_24, sizeof(param->unk_24));
        param->unk_9e = 0;
    } else {
        func_arm7_027c05e0();                   /* result unused, as in the original */
        param->unk_20 = buf->status->wepMode;
        param->unk_22 = buf->status->wepKeyId;
        Mem_CopyBytes_Arm7(buf->status->wepKey, param->unk_24, sizeof(param->unk_24));
        param->unk_9e = 1;
    }
    param->unk_74 = 1;
    param->unk_76 = 1;
    if (buf->status->unk_e2 == 1) {
        param->unk_78 = 0;
    } else {
        param->unk_78 = 16;
    }
    param->unk_7a = 10;
    zeroMask = 0;
    Mem_FillHalfwords_Arm7(zeroMask, param->unk_7c, sizeof(param->unk_7c));
    fullMask = 0xffff;
    Mem_FillHalfwords_Arm7(fullMask, param->unk_84, sizeof(param->unk_84));
    param->unk_9c = buf->status->unk_1ea;
    result = func_arm7_027c21f0(cmd);
    if (result[2] == 0) {
        return TRUE;
    }
    Wmsp_ReplyFailed(apiid, 0x200, result[2]);
    return FALSE;
}
