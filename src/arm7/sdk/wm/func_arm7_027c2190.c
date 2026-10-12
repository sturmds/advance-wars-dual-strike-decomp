#include "sdk/wmsp.h"

/* WL command 0x207, one argument word. Returns the reply header after the arguments. */
u16 *func_arm7_027c2190(WlCommand *cmd, u16 arg)
{
    u16 *reply;
    u16 *slot;

    cmd->reserved[0] = 0;
    cmd->reserved[1] = 0;
    cmd->reserved[2] = 0;
    cmd->reserved[3] = 0;
    cmd->reserved[4] = 0;
    cmd->reserved[5] = 0;
    cmd->length = 0x207;
    cmd->index = 1;
    cmd->words[0] = arg;
    slot = (u16 *)cmd + cmd->index;
    reply = slot + 8;
    slot[8] = cmd->length;
    reply[1] = 1;
    Wmsp_SendWlCommand(cmd);
    return reply;
}
