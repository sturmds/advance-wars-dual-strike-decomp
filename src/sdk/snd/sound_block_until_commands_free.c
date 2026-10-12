#include "sdk/snd.h"

int func_0211005c(void);

/* Blocks until count commands are free. If the commands in flight would cover it, wait for the ARM7 to return them; otherwise send the queued ones first. */
void Sound_BlockUntilCommandsFree(int count) {
  int freeCount = Sound_FreeCommandCount();

  if (freeCount >= count) {
    return;
  }
  if (freeCount + func_0211005c() >= count) {
    while (SND_RecvCommandReply(0) != 0) {
    }
    if (Sound_FreeCommandCount() >= count) {
      return;
    }
  } else {
    SND_FlushCommand(SOUND_SEND_WAIT_IF_FULL);
  }
  Sound_PingArm7Driver();
  do {
    SND_RecvCommandReply(1);
  } while (Sound_FreeCommandCount() < count);
}
