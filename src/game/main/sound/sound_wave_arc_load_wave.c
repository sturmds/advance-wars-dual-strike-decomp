#include "types.h"
#include "game/sound_heap.h"
#include "game/sound_wave_arc.h"
#include "sdk/snd.h"
#include "sdk/os.h"

u32 func_02110a90(SoundWaveArc* wave);
s32 SoundArc_ReadFile(u32 fileNo, void* dest, u32 size, u32 offset);
void* SoundHeap_AllocItem(SoundHeap* heap, u32 size,
                          void (*onUnload)(void* payload, u32 arg1, u32 arg2,
                                           u32 arg3),
                          u32 arg3, u32 arg4);
void SoundWaveArc_UnloadWave(void* data, u32 size, SoundWaveArc* wave,
                             u32 waveNo);

/* Word index of SoundWaveArc.table. Needed to match: the file offsets are
 * indexed from the start of the archive, not from the table. */
#define WAVE_TABLE_WORD 15

/* Loads wave number waveNo of a wave archive from file fileNo of the current
 * sound archive into heap, and records its address in the wave table. Does
 * nothing when the wave is already loaded. Returns FALSE when the heap is
 * missing or full or the read falls short. */
BOOL SoundWaveArc_LoadWave(SoundWaveArc* wave, u32 waveNo, u32 fileNo,
                           SoundHeap* heap) {
  u32 size;
  u32* entry;
  u32 waveCount;
  u8* data;
  u32 end;
  u32 start;

  if (SND_GetWaveDataAddress(wave, waveNo)) {
    return TRUE;
  }
  waveCount = func_02110a90(wave);
  entry = (u32*)wave + (wave->count + waveNo);
  start = entry[WAVE_TABLE_WORD];
  if (waveNo < waveCount - 1) {
    end = entry[WAVE_TABLE_WORD + 1];
  } else {
    end = wave->fileSize;
  }
  size = end - start;
  if (heap == NULL) {
    return FALSE;
  }
  data = SoundHeap_AllocItem(
      heap, size + 32, (void (*)(void*, u32, u32, u32))SoundWaveArc_UnloadWave,
      (u32)wave, waveNo);
  if (data == NULL) {
    return FALSE;
  }
  if (size != SoundArc_ReadFile(fileNo, data, size, start)) {
    return FALSE;
  }
  DC_StoreRange(data, size);
  SND_SetWaveDataAddress(wave, waveNo, data);
  return TRUE;
}
