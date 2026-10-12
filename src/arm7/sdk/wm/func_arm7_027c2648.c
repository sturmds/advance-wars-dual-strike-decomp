#include "sdk/wmsp.h"

u16 *func_arm7_027c2648(WlCommand *cmd, const void *src, int arg)
{
    u16 *reply;
    u16 *slot;

    cmd->reserved[0] = 0;
    cmd->reserved[1] = 0;
    cmd->reserved[2] = 0;
    cmd->reserved[3] = 0;
    cmd->reserved[4] = 0;
    cmd->reserved[5] = 0;
    cmd->length = 5;
    cmd->index = 4;
    func_arm7_037fde40(src, &cmd->words[0], 6);
    cmd->words[3] = arg;
    slot = (u16 *)cmd + cmd->index;
    reply = slot + 8;
    slot[8] = cmd->length;
    reply[1] = 4;
    Wmsp_SendWlCommand(cmd);
    return reply;
}
