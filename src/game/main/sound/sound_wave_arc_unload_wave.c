#include "types.h"
#include "game/sound_wave_arc.h"

void *func_02110a10(SoundWaveArc *wave, u32 waveNo);
void func_02110a58(SoundWaveArc *wave, u32 waveNo, void *data);
void SND_InvalidateWaveData(void *start, void *end);

/* Unload callback of a wave loaded by SoundWaveArc_LoadWave. Clears the
 * wave in its archive if the archive still points at it, then passes the
 * freed range (size bytes from data) to SND_InvalidateWaveData. */
void SoundWaveArc_UnloadWave(void *data, u32 size, SoundWaveArc *wave, u32 waveNo)
{
    if (data == func_02110a10(wave, waveNo)) {
        func_02110a58(wave, waveNo, NULL);
    }
    SND_InvalidateWaveData(data, (u8 *)data + size);
}
