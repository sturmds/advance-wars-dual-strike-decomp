#include "types.h"

void Sound_PlaySe(u32 soundId);
void Sound_PlaySeAtVolume(u32 soundId, int volume);

extern u32 data_ov003_02369c08;

/* Plays sound 163 for the flags the overlay set since the last call (bit 0
 * at full volume, bit 1 quieter), then clears them. */
void Ov003_PlayPendingSound(void) {
  u32 flags = data_ov003_02369c08;

  if (flags & 1) {
    Sound_PlaySe(163);
  } else if (flags & 2) {
    Sound_PlaySeAtVolume(163, 95);
  }
  data_ov003_02369c08 = 0;
}
