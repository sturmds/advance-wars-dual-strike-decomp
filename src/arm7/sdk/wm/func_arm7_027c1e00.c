#include "sdk/wmsp.h"

/* WL command 0x309, four argument words. Returns the reply header after the arguments. */
u16* func_arm7_027c1e00(WlCommand* cmd, u16 arg0, u16 arg1, u16 arg2,
                        u16 arg3) {
  u16* reply;
  u16* slot;

  cmd->reserved[0] = 0;
  cmd->reserved[1] = 0;
  cmd->reserved[2] = 0;
  cmd->reserved[3] = 0;
  cmd->reserved[4] = 0;
  cmd->reserved[5] = 0;
  cmd->command = 0x309;
  cmd->length = 4;
  cmd->words[0] = arg0;
  cmd->words[1] = arg1;
  cmd->words[2] = arg2;
  cmd->words[3] = arg3;
  slot = (u16*)cmd + cmd->length;
  reply = slot + 8;
  slot[8] = cmd->command;
  reply[1] = 1;
  Wmsp_SendWlCommand(cmd);
  return reply;
}
