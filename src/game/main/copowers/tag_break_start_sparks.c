#include "types.h"
#include "game/screen_other_map.h"

extern u16 data_0229f330[];        /* layer 0 map to show */
extern u8 gTagBreakSparkPalette[]; /* 16-color palette for it */

void Mem_Copy32(const void* src, void* dest, u32 size);
void Palette_Load(const void* src, u32 offset, u32 size);
void Gfx_RequestLayerCopy(int layer);
void SparkBurst_Start(int x, int y, int arg2);
void Sound_PlaySe(u32 soundId);

#define SE_SPARKS 234

/* Step of the tag break script: shows the layer 0 map from data_0229f330 in
 * palette slot 1, then starts two spark bursts and their sound. */
void TagBreak_StartSparks(void) {
  int i;

  Mem_Copy32(data_0229f330, data_027c0280->layer0Map, 0x800);
  Palette_Load(gTagBreakSparkPalette, 32, 32);
  for (i = 0; i < 1024; i++) {
    data_027c0280->layer0Map[i] += 1 << 12;
  }
  Gfx_RequestLayerCopy(0);
  SparkBurst_Start(40, 48, 0);
  SparkBurst_Start(158, 86, 0);
  Sound_PlaySe(SE_SPARKS);
}
