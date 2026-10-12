#include "sdk/wmsp.h"

u16* func_arm7_027c25c8(WlCommand* cmd, const void* src, int arg0, int arg1) {
  u16* reply;
  u16* slot;

  cmd->reserved[0] = 0;
  cmd->reserved[1] = 0;
  cmd->reserved[2] = 0;
  cmd->reserved[3] = 0;
  cmd->reserved[4] = 0;
  cmd->reserved[5] = 0;
  cmd->command = 6;
  cmd->length = 5;
  Mem_CopyHalfwords_Arm7(src, &cmd->words[0], 6);
  cmd->words[3] = arg0;
  cmd->words[4] = arg1;
  slot = (u16*)cmd + cmd->length;
  reply = slot + 8;
  slot[8] = cmd->command;
  reply[1] = 3;
  Wmsp_SendWlCommand(cmd);
  return reply;
}
