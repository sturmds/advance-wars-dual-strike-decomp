#include "sdk/snd_hw.h"

void SndHw_SetOutputControl(int leftOutput, int rightOutput, int ch1Direct,
                            int ch3Direct) {
  int enabled = (AUDIO_MIXER_ROUTE_BYTE & AUDIO_MIXER_MASTER_ON) != 0;

  AUDIO_MIXER_ROUTE_BYTE = leftOutput | ((enabled << 7) | (ch3Direct << 5) |
                                         (ch1Direct << 4) | (rightOutput << 2));
}
