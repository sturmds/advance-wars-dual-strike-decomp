#include "sdk/snd.h"

/* Sends one word on the PXI FIFO; negative when the FIFO is full. */
int PXI_SendWordByFifo(int tag, u32 data, int err);

void Sound_PingArm7Driver(void) {
  while (PXI_SendWordByFifo(SOUND_FIFO_TAG, 0, 0) < 0) {
  }
}
