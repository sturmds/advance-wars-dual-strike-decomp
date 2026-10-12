#include "sdk/wmsp.h"

/* WL command 10: a zero word, three argument words, then 16 bytes of data. Returns the reply header after the arguments. */
u16 *func_arm7_027c2468(WlCommand *cmd, u16 arg1, u16 arg2, u16 arg3, const void *data)
{
    u16 *reply;
    u16 *slot;

    cmd->reserved[0] = 0;
    cmd->reserved[1] = 0;
    cmd->reserved[2] = 0;
    cmd->reserved[3] = 0;
    cmd->reserved[4] = 0;
    cmd->reserved[5] = 0;
    cmd->length = 10;
    cmd->index = 12;
    cmd->words[0] = 0;
    cmd->words[1] = arg1;
    cmd->words[2] = arg2;
    cmd->words[3] = arg3;
    func_arm7_037fde40(data, &cmd->words[4], 16);
    slot = (u16 *)cmd + cmd->index;
    reply = slot + 8;
    slot[8] = cmd->length;
    reply[1] = 18;
    Wmsp_SendWlCommand(cmd);
    return reply;
}
