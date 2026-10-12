#include "types.h"
#include "game/Unk_02088d04.h"

void Gfx_ClearLayerMap(int layer);
void Mem_Copy32(const void* src, void* dest, u32 size);
void Gfx_RequestLayerCopy(int layer);

extern Unk_027c0280* data_027c0280;

/* Copies the second 0x800-byte page of layer 1's tile map over the first and
 * redraws layers 0 and 1. Its caller runs it when the world map task exists
 * (guess: the world map is the screen that uses it). */
void CampaignMap_CopyLayer1SecondPage(void) {
  Gfx_ClearLayerMap(0);
  Mem_Copy32((u8*)data_027c0280->layer1Map + 0x800, data_027c0280->layer1Map,
             0x800);
  Gfx_RequestLayerCopy(0);
  Gfx_RequestLayerCopy(1);
}
