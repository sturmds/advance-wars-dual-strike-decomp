#include "sdk/wmsp.h"

u16* func_arm7_027c2648(WlCommand* cmd, const void* src, int arg) {
  u16* reply;
  u16* slot;

  cmd->reserved[0] = 0;
  cmd->reserved[1] = 0;
  cmd->reserved[2] = 0;
  cmd->reserved[3] = 0;
  cmd->reserved[4] = 0;
  cmd->reserved[5] = 0;
  cmd->command = 5;
  cmd->length = 4;
  Mem_CopyHalfwords_Arm7(src, &cmd->words[0], MAC_ADDRESS_SIZE);
  cmd->words[3] = arg;
  slot = (u16*)cmd + cmd->length;
  reply = slot + 8;
  slot[8] = cmd->command;
  reply[1] = 4;
  Wmsp_SendWlCommand(cmd);
  return reply;
}
