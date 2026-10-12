#include "sdk/wmsp.h"

u16 *func_arm7_027c2890(WlCommand *cmd, int arg0, int arg1, int arg2)
{
    u16 *reply;
    u16 *slot;

    cmd->reserved[0] = 0;
    cmd->reserved[1] = 0;
    cmd->reserved[2] = 0;
    cmd->reserved[3] = 0;
    cmd->reserved[4] = 0;
    cmd->reserved[5] = 0;
    cmd->command = 1;
    cmd->length = 3;
    cmd->words[0] = arg0;
    cmd->words[1] = arg1;
    cmd->words[2] = arg2;
    slot = (u16 *)cmd + cmd->length;
    reply = slot + 8;
    slot[8] = cmd->command;
    reply[1] = 1;
    Wmsp_SendWlCommand(cmd);
    return reply;
}
