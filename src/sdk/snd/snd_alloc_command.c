#include "sdk/snd.h"

int func_0211005c(void);
int SND_FlushCommand(u32 flags);
int func_0210dc64(void);

SndCommand* SND_AllocCommand(int block) {
  SndCommand* command;

  if (func_0210dc64() != 0) {
    return NULL;
  }
  command = SND_PopFreeCommand();
  if (command != NULL) {
    return command;
  }
  if (!(block & 1)) {
    return NULL;
  }
  if (func_0211005c() > 0) {
    while (SND_RecvCommandReply(0) != NULL) {
    }
    command = SND_PopFreeCommand();
    if (command != NULL) {
      return command;
    }
  } else {
    SND_FlushCommand(1);
  }
  Sound_PingArm7Driver();
  do {
    SND_RecvCommandReply(1);
    command = SND_PopFreeCommand();
  } while (command == NULL);
  return command;
}
