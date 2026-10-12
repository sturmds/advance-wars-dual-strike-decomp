#include "sdk/wmsp.h"

u16* func_arm7_027c27d4(WlCommand* cmd, u32 size, const void* mac, int arg0,
                        const void* block32, u16 arg1, const void* block16,
                        u16 arg2) {
  u16* reply;
  u16* slot;

  cmd->reserved[0] = 0;
  cmd->reserved[1] = 0;
  cmd->reserved[2] = 0;
  cmd->reserved[3] = 0;
  cmd->reserved[4] = 0;
  cmd->reserved[5] = 0;
  cmd->command = 2;
  cmd->length = 31;
  Mem_CopyHalfwords_Arm7(mac, &cmd->words[0], 6);
  cmd->words[3] = arg0;
  Mem_CopyHalfwords_Arm7(block32, &cmd->words[4], 32);
  cmd->words[20] = arg1;
  Mem_CopyHalfwords_Arm7(block16, &cmd->words[21], 16);
  cmd->words[29] = arg2;
  cmd->words[30] = 0;
  slot = (u16*)cmd + cmd->length;
  reply = slot + 8;
  slot[8] = cmd->command;
  reply[1] = (size >> 1) - 44;
  Wmsp_SendWlCommand(cmd);
  return reply;
}
