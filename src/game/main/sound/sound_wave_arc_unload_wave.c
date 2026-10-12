#include "types.h"
#include "game/sound_wave_arc.h"
#include "sdk/snd.h"

void SND_InvalidateWaveData(void* start, void* end);

/* Unload callback of a wave loaded by SoundWaveArc_LoadWave. Clears the
 * wave in its archive if the archive still points at it, then passes the
 * freed range (size bytes from data) to SND_InvalidateWaveData. */
void SoundWaveArc_UnloadWave(void* data, u32 size, SoundWaveArc* wave,
                             u32 waveNo) {
  if (data == SND_GetWaveDataAddress(wave, waveNo)) {
    SND_SetWaveDataAddress(wave, waveNo, NULL);
  }
  SND_InvalidateWaveData(data, (u8*)data + size);
}
