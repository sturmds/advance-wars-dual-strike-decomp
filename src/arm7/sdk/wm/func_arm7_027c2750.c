#include "sdk/wmsp.h"

u16 *func_arm7_027c2750(WlCommand *cmd, int arg, const void *src)
{
    u16 *reply;
    u16 *slot;

    cmd->reserved[0] = 0;
    cmd->reserved[1] = 0;
    cmd->reserved[2] = 0;
    cmd->reserved[3] = 0;
    cmd->reserved[4] = 0;
    cmd->reserved[5] = 0;
    cmd->length = 3;
    cmd->index = 0x22;
    cmd->words[0] = arg;
    cmd->words[1] = 0;
    func_arm7_037fde40(src, &cmd->words[2], 0x44);
    slot = (u16 *)cmd + cmd->index;
    reply = slot + 8;
    slot[8] = cmd->length;
    reply[1] = 5;
    func_arm7_027c0560(cmd);
    return reply;
}
