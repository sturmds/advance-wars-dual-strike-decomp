#include "sdk/snd.h"

int SND_FlushCommand(u32 flags);

/* aligned to 32: no symbol covers the bytes before it */
static __attribute__((aligned(32))) SndSharedWork data_022a9cc0;
SndCommand data_022a9f40[256]; /* command pool */
extern SndSharedWork* data_022ab7a0;
extern int data_022a9c7c;

void SND_CommandInit(void) {
  int i;
  SndCommand* command;

  Sound_WaitForArm7Listener();
  data_022a9c68 = data_022a9f40;
  for (i = 0; i < 255; i++) {
    data_022a9f40[i].next = &data_022a9f40[i + 1];
  }
  data_022a9f40[255].next = NULL;
  data_022a9c78 = &data_022a9f40[255];
  data_022a9c70 = NULL;
  data_022a9c74 = NULL;
  data_022a9c84 = 0;
  data_022a9c7c = 0;
  data_022a9c80 = 0;
  data_022a9c88 = 1;
  data_022a9c6c = 0;
  data_022ab7a0 = &data_022a9cc0;
  Sound_ResetArm7StatusBlock(&data_022a9cc0);

  command = SND_AllocCommand(1);
  if (command == NULL) {
    return;
  }
  command->id = 27;
  command->args[0] = (u32)data_022ab7a0;
  Sound_LinkCommandAtTail(command);
  SND_FlushCommand(1);
}
