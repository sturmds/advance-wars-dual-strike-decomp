#include "sdk/snd.h"

int data_022a9c50; /* SND_Init has run */
/* lock around the command lists; size: up to the next symbol */
u8 data_022a9c54[0x14];

void func_0210e0c0(void* mutex);
void SND_CommandInit(void);

void SND_Init(void) {
  if (data_022a9c50) {
    return;
  }
  data_022a9c50 = 1;
  func_0210e0c0(&data_022a9c54);
  SND_CommandInit();
  SoundAlarm_ClearAll();
}
