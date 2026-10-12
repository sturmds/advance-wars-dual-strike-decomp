#include "sdk/wmsp.h"

u16* func_arm7_027c2750(WlCommand* cmd, int arg, const void* src) {
  u16* reply;
  u16* slot;

  cmd->reserved[0] = 0;
  cmd->reserved[1] = 0;
  cmd->reserved[2] = 0;
  cmd->reserved[3] = 0;
  cmd->reserved[4] = 0;
  cmd->reserved[5] = 0;
  cmd->command = 3;
  cmd->length = 0x22;
  cmd->words[0] = arg;
  cmd->words[1] = 0;
  Mem_CopyHalfwords_Arm7(src, &cmd->words[2], 0x44);
  slot = (u16*)cmd + cmd->length;
  reply = slot + 8;
  slot[8] = cmd->command;
  reply[1] = 5;
  Wmsp_SendWlCommand(cmd);
  return reply;
}
