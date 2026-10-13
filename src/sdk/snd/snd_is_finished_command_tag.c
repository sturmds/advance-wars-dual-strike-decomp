#include "sdk/snd.h"
#include "sdk/os.h"

SndCommand* data_022a9c70; /* queued, unflushed commands: head */
u32 data_022a9c6c;         /* tag of the last finished command batch */

/* Tags count up and wrap, so a tag counts as reached when it trails the finished tag by under half the range. */
int SND_IsFinishedCommandTag(u32 tag) {
  int irq = OS_DisableInterrupts();
  u32 finished = data_022a9c6c;
  int result;

  if (tag > finished) {
    if ((tag - finished) < 0x80000000) {
      result = 0;
    } else {
      result = 1;
    }
  } else {
    result = (finished - tag) < 0x80000000;
  }
  OS_RestoreInterrupts(irq);
  return result;
}
