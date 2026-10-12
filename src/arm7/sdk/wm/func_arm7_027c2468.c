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
    cmd->command = 10;
    cmd->length = 12;
    cmd->words[0] = 0;
    cmd->words[1] = arg1;
    cmd->words[2] = arg2;
    cmd->words[3] = arg3;
    Mem_CopyHalfwords_Arm7(data, &cmd->words[4], 16);
    slot = (u16 *)cmd + cmd->length;
    reply = slot + 8;
    slot[8] = cmd->command;
    reply[1] = 18;
    Wmsp_SendWlCommand(cmd);
    return reply;
}
