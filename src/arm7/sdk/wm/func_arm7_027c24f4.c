#include "sdk/wmsp.h"

/* WL command 9: 23 argument words, then size bytes of data. Returns the reply header after the arguments. */
u16 *func_arm7_027c24f4(WlCommand *cmd, u16 arg0, const void *block, u16 arg17, u16 arg18, u16 arg19,
                        u16 arg20, u16 arg21, u16 size, const void *data)
{
    u16 *reply;
    u16 *slot;

    cmd->reserved[0] = 0;
    cmd->reserved[1] = 0;
    cmd->reserved[2] = 0;
    cmd->reserved[3] = 0;
    cmd->reserved[4] = 0;
    cmd->reserved[5] = 0;
    cmd->command = 9;
    cmd->length = (s32)func_arm7_03806d4c(size + 1, 2) + 23;
    cmd->words[0] = arg0;
    Mem_CopyHalfwords_Arm7(block, &cmd->words[1], 32);
    cmd->words[17] = arg17;
    cmd->words[18] = arg18;
    cmd->words[19] = arg19;
    cmd->words[20] = arg20;
    cmd->words[21] = arg21;
    cmd->words[22] = size;
    Mem_CopyHalfwords_Arm7(data, &cmd->words[23], size);
    slot = (u16 *)cmd + cmd->length;
    reply = slot + 8;
    slot[8] = cmd->command;
    reply[1] = 1;
    Wmsp_SendWlCommand(cmd);
    return reply;
}
