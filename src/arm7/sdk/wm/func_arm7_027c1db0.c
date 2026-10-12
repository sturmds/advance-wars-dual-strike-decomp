#include "sdk/wmsp.h"

/* WL command `id` with no argument words. Returns the reply header. */
u16* func_arm7_027c1db0(WlCommand* cmd, int id, int replyLength) {
  u16* reply;
  u16* slot;

  cmd->reserved[0] = 0;
  cmd->reserved[1] = 0;
  cmd->reserved[2] = 0;
  cmd->reserved[3] = 0;
  cmd->reserved[4] = 0;
  cmd->reserved[5] = 0;
  cmd->command = id;
  cmd->length = 0;
  slot = (u16*)cmd + cmd->length;
  reply = slot + 8;
  slot[8] = cmd->command;
  reply[1] = replyLength;
  Wmsp_SendWlCommand(cmd);
  return reply;
}
