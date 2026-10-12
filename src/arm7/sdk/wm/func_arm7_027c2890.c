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
    cmd->length = 1;
    cmd->index = 3;
    cmd->words[0] = arg0;
    cmd->words[1] = arg1;
    cmd->words[2] = arg2;
    slot = (u16 *)cmd + cmd->index;
    reply = slot + 8;
    slot[8] = cmd->length;
    reply[1] = 1;
    func_arm7_027c0560(cmd);
    return reply;
}
