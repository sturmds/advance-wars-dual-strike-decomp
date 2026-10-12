#include "sdk/os.h"
#include "sdk/snd.h"

extern SndCommand* data_022a9c68;
extern u32 data_022a9c6c;
extern SndCommand* data_022a9c78;
extern int data_022a9c7c;
extern int data_022a9c84;
extern SndCommand* data_022a9c8c[9];

SndCommand* SND_RecvCommandReply(int block) {
  int intrState = OS_DisableInterrupts();
  SndCommand* command;
  SndCommand* last;
  int index;

  if (block & 1) {
    while (Sound_LastDoneCommandTag() == data_022a9c6c) {
      OS_RestoreInterrupts(intrState);
      OS_SpinWait(100);
      intrState = OS_DisableInterrupts();
    }
  } else if (Sound_LastDoneCommandTag() == data_022a9c6c) {
    OS_RestoreInterrupts(intrState);
    return NULL;
  }

  index = data_022a9c7c;
  command = data_022a9c8c[index];
  index++;
  data_022a9c7c = index;
  if (index > 8) {
    data_022a9c7c = 0;
  }

  last = command;
  if (command->next != NULL) {
    do {
      last = last->next;
    } while (last->next != NULL);
  }

  if (data_022a9c78 != NULL) {
    data_022a9c78->next = command;
  } else {
    data_022a9c68 = command;
  }
  data_022a9c78 = last;
  data_022a9c84--;
  data_022a9c6c++;
  OS_RestoreInterrupts(intrState);
  return command;
}
