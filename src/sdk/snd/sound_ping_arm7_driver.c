#include "sdk/snd.h"
#include "sdk/pxi.h"

/* Sends one word on the PXI FIFO; negative when the FIFO is full. */

void Sound_PingArm7Driver(void) {
  while (PXI_SendWordByFifo(SOUND_FIFO_TAG, 0, 0) < 0) {
  }
}
