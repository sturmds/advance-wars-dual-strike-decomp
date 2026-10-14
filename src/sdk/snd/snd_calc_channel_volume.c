#include "sdk/snd.h"

static const u8 data_02151634[] = {
#include "data/main/data_02151634.inc.c"
};

u16 SND_CalcChannelVolume(int volume) {
  int shift;
  u8 level;

  if (volume < -723) {
    volume = -723;
  } else if (volume > 0) {
    volume = 0;
  }
  level = data_02151634[volume + 723];
  if (volume < -240) {
    shift = 3;
  } else if (volume < -120) {
    shift = 2;
  } else if (volume < -60) {
    shift = 1;
  } else {
    shift = 0;
  }
  return level | (shift << 8);
}
