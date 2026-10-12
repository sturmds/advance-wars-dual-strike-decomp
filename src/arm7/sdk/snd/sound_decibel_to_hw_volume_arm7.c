#include "types.h"

/* BIOS GetVolumeTable: entry index of the decibel-to-volume table. */
extern int func_arm7_037fec60(int index);

/* Maps a volume from -723 to 0 to the hardware volume (low byte) and volume shift (bits 8-9). */
u16 Sound_DecibelToHwVolume_Arm7(int volume) {
  int shift;
  int level;

  if (volume < -723) {
    volume = -723;
  } else if (volume > 0) {
    volume = 0;
  }
  level = func_arm7_037fec60(volume + 723);
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
