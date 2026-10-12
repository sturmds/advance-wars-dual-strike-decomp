#include "sdk/wmsp.h"

u16 *func_arm7_027c1db0(WlCommand *cmd, int id, int replyLength);

/* WL command 0x304, no arguments. Returns the reply header. */
u16 *func_arm7_027c1ed0(WlCommand *cmd)
{
    return func_arm7_027c1db0(cmd, 0x304, 1);
}
