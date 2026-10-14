#include "types.h"
#include "game/Unk_02088d04.h"
#include "game/Unk_0229fb30.h"

extern Unk_027c0280* data_027c0280;

/* The tile map and palette this step loads sit in main's bss at gMenuState
 * and data_0229fb30 (shared buffers). */
extern u8 gMenuState[];

void Mem_Copy32(const void* src, void* dst, u32 size);
void Palette_Load(const void* src, int offset, int size);
void Gfx_RequestLayerCopy(int layer);
void SparkBurst_Start(int x, int y, int waitFrames);
void Sound_PlaySe(int id);

/* Step of the power screen script: replaces layer 0's tile map, loads a
 * palette into slot 1 and points the map at it (+0x1000 per tile), then sets
 * off two spark bursts with a sound. */
void PowerScreen_SwapMapWithSparks(void) {
  int i;

  Mem_Copy32(gMenuState, data_027c0280->layer0Map, BG_SCREEN_SIZE);
  Palette_Load(&data_0229fb30, 32, 32);
  for (i = 0; i < BG_SCREEN_ENTRIES; i++) {
    data_027c0280->layer0Map[i] += 0x1000;
  }
  Gfx_RequestLayerCopy(0);
  SparkBurst_Start(42, 146, 3);
  SparkBurst_Start(172, 112, 0);
  Sound_PlaySe(234);
}
