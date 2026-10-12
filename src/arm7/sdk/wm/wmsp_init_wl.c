#include "sdk/wmsp.h"

u16 *func_arm7_027c1ea0(WlCommand *cmd);
u16 *func_arm7_027c1ed0(WlCommand *cmd);
u16 *func_arm7_027c1efc(WlCommand *cmd);
u16 *func_arm7_027c1f40(WlCommand *cmd);
u16 *func_arm7_027c1f58(WlCommand *cmd);
u16 *func_arm7_027c2124(WlCommand *cmd, u16 arg0, u16 arg1, u16 arg2);
u16 *func_arm7_027c2064(WlCommand *cmd, u16 arg);
void WMSP_InitConfigRegisters(void);
u16 PickWirelessChannels(u16 value);
void func_arm7_037fdf68(const void *src, void *dest, u32 size); /* copy bytes */

/* Runs the WL start-up commands and reads the usable channels, the firmware
   versions and the MAC address into WMStatus. On a failed command, returns
   FALSE with that command's id and result. */
int WMSP_InitWl(u16 *wlCmdID, u16 *wlResult)
{
    WmspBuf *buf = func_arm7_027c05e0()->buf;
    u32 cmd[0x80];
    u16 *result;
    u16 channels;

    result = func_arm7_027c1ed0((WlCommand *)cmd);
    if (result[2] != 0) {
        *wlCmdID = 0x304;
        *wlResult = result[2];
        return FALSE;
    }
    result = func_arm7_027c1efc((WlCommand *)cmd);
    if (result[2] != 0) {
        *wlCmdID = 0x302;
        *wlResult = result[2];
        return FALSE;
    }
    WMSP_InitConfigRegisters();
    result = func_arm7_027c1f40((WlCommand *)cmd);
    if (result[2] != 0) {
        *wlCmdID = 0x283;
        *wlResult = result[2];
        return FALSE;
    }
    channels = result[3];
    buf->status->enableChannel = channels;
    buf->status->allowedChannel = PickWirelessChannels(channels >> 1);
    func_arm7_027c2124((WlCommand *)cmd, 0xffff, 40, 5);
    buf->status->unk_7b0 = 0x1ff621;
    buf->status->unk_1e8 = 2;
    buf->status->unk_1ea = 1;
    result = func_arm7_027c1ea0((WlCommand *)cmd);
    if (result[2] != 0) {
        *wlCmdID = 0x306;
        *wlResult = result[2];
        return FALSE;
    }
    func_arm7_037fde40(&result[3], buf->status->wlVersion, sizeof(buf->status->wlVersion));
    buf->status->macVersion = result[7];
    buf->status->bbpVersion[0] = result[8];
    buf->status->bbpVersion[1] = result[9];
    buf->status->rfVersion = result[10];
    result = func_arm7_027c1f58((WlCommand *)cmd);
    if (result[2] != 0) {
        *wlCmdID = 0x281;
        *wlResult = result[2];
        return FALSE;
    }
    func_arm7_037fdf68(&result[3], buf->status->macAddress, sizeof(buf->status->macAddress));
    result = func_arm7_027c2064((WlCommand *)cmd, 1);
    if (result[2] != 0) {
        *wlCmdID = 0x215;
        *wlResult = result[2];
        return FALSE;
    }
    return TRUE;
}
