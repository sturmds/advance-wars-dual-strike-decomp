#include "types.h"
#include "sdk/snd.h"

void func_0210fec0(void);
void func_0210fed4(void);

/* Returns the loaded address of wave waveNo, or NULL when it is not loaded.
 * An entry below 0x2000000 is an offset from the archive start. */
void* SND_GetWaveDataAddress(SoundWaveArc* arc, u32 waveNo) {
  u32 entry;
  u8* wave;

  func_0210fed4();
  entry = arc->table[waveNo];
  if (entry != 0) {
    wave = (u8*)entry;
    if (entry < 0x2000000) {
      wave = (u8*)arc + entry;
    }
  } else {
    wave = NULL;
  }
  func_0210fec0();
  return wave;
}
