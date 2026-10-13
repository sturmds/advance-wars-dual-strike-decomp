#ifndef GAME_UNK_022A8A10_H
#define GAME_UNK_022A8A10_H

#include "types.h"
#include "game/sound_handle.h"
#include "sdk/os_common.h"

/* State of one running sound output, at data_022a8a10 (main bss). Only the
 * fields the code in src/ uses. */
typedef struct Unk_022a8a10 {
  int running; /* 0x00: nonzero while started */
  int unk_04;  /* 0x04: nonzero stops the per-frame volume update */
  u8 pad_08[4];
  void* bufferL;  /* 0x0c: output buffer, one per side */
  void* bufferR;  /* 0x10 */
  u32 bufferSize; /* 0x14 */
  int unk_18;     /* 0x18: cleared when the buffers are cleared */
  int unk_1c;     /* 0x1c: nonzero when channels were locked */
  int unk_20;     /* 0x20: first argument of the volume call */
  int unk_24; /* 0x24: second argument of the volume call, nonzero when set up */
  int alarm; /* 0x28: SND alarm number (bit of the alarm mask), negative for none */
  u8 pad_2c[0xc];
  SoundFader fader;   /* 0x38 */
  int closeWhenFaded; /* 0x48: close the output once the fader ends */
  int appliedVolume;  /* 0x4c: last volume sent to the hardware */
} Unk_022a8a10;

extern Unk_022a8a10 data_022a8a10;
extern OSMessageQueue data_022a89fc;

#endif
