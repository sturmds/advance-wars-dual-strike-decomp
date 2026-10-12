#include "sdk/wmsp.h"

/* WL command 0x100: copies 0x30 bytes from params, then clears some of its fields. Returns the reply header after the arguments. */
u16 *func_arm7_027c23b0(WlCommand *cmd, u16 *params)
{
    u16 *reply;
    u16 *slot;

    cmd->reserved[0] = 0;
    cmd->reserved[1] = 0;
    cmd->reserved[2] = 0;
    cmd->reserved[3] = 0;
    cmd->reserved[4] = 0;
    cmd->reserved[5] = 0;
    cmd->length = 0x100;
    cmd->index = 24;
    Mem_CopyHalfwords_Arm7(params, &cmd->words[0], 48);
    params[1] = 0;
    params[2] = 0;
    params[4] = 0;
    params[5] = 0;
    params[6] = 0;
    params[8] = 0;
    params[9] = 0;
    params[10] = 0;
    params[11] = 0;
    params[18] = 0;
    params[19] = 0;
    params[20] = 0;
    params[21] = 0;
    slot = (u16 *)cmd + cmd->index;
    reply = slot + 8;
    slot[8] = cmd->length;
    reply[1] = 2;
    Wmsp_SendWlCommand(cmd);
    return reply;
}
