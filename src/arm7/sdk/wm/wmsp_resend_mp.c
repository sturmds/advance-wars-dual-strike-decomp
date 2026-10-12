#include "sdk/wmsp.h"

u16 func_arm7_027c6020(u32 parentSize, u32 childSize, u32 childBitmap);
u16 *func_arm7_027c22a8(WlCommand *cmd, u16 arg0, u16 arg1, u16 arg2, u16 arg3, u16 arg4, u16 arg5,
                        u16 arg6, u16 arg7, u32 arg8);

/*
 * Request slot 42 on a parent: issues WL command 0x102 again for the children in
 * arg, with the sizes and flags WMSP_SendMp kept from the last MP send and
 * 0x800c as the first word instead of the data.
 */
void WMSP_ResendMp(u16 arg)
{
    WMStatus *status = func_arm7_027c05e0()->buf->status;
    int irqState;
    u16 now;
    u32 flags;
    u32 pollBitmap;
    u16 time;
    u32 cmd[0x81];

    irqState = Cpu_MaskIrq_Arm7();
    Cpu_RestoreIrqBit_Arm7(irqState);
    now = W_US_COUNT0;
    flags = status->unk_3e;
    pollBitmap = arg & status->childBitmap;
    if (status->unk_36 == 1) {
        time = func_arm7_027c6020(status->unk_42, status->unk_40, pollBitmap);
        flags |= 0x8000;
    } else {
        time = 0;
        flags &= ~0x8000;
    }
    func_arm7_027c22a8((WlCommand *)cmd, 0x800c, 0, 0, pollBitmap, time, now, 0, flags, 0);
}
