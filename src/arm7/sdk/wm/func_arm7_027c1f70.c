#include "sdk/wmsp.h"

/* WL command 0x245: a size word, then size bytes of data. Returns the reply header after the arguments. */
u16 *func_arm7_027c1f70(WlCommand *cmd, int size, const void *data)
{
    u16 *reply;
    u16 *slot;

    func_arm7_027c05e0();
    cmd->reserved[0] = 0;
    cmd->reserved[1] = 0;
    cmd->reserved[2] = 0;
    cmd->reserved[3] = 0;
    cmd->reserved[4] = 0;
    cmd->reserved[5] = 0;
    cmd->length = 0x245;
    cmd->index = (s32)func_arm7_03806d4c(size + 1, 2) + 1;
    cmd->words[0] = size;
    Mem_CopyHalfwords_Arm7(data, &cmd->words[1], size);
    slot = (u16 *)cmd + cmd->index;
    reply = slot + 8;
    slot[8] = cmd->length;
    reply[1] = 1;
    Wmsp_SendWlCommand(cmd);
    return reply;
}
