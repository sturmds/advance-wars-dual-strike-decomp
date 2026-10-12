#include "sdk/wmsp.h"

/* WL command 0x200; sends 0x48 argument words as they stand in the buffer. Returns the reply header after the arguments. */
u16* func_arm7_027c21f0(WlCommand* cmd) {
  u16* reply;
  u16* slot;

  cmd->reserved[0] = 0;
  cmd->reserved[1] = 0;
  cmd->reserved[2] = 0;
  cmd->reserved[3] = 0;
  cmd->reserved[4] = 0;
  cmd->reserved[5] = 0;
  cmd->command = 0x200;
  cmd->length = 0x48;
  slot = (u16*)cmd + cmd->length;
  reply = slot + 8;
  slot[8] = cmd->command;
  reply[1] = 1;
  Wmsp_SendWlCommand(cmd);
  return reply;
}
