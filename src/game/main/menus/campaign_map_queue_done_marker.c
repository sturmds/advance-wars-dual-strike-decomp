#include "types.h"

void Sprite_QueueAt(int list, int x, int y, void* data, int attr, int tile);

extern u8 data_027c0040;

/* Queues the world map marker sprite at screen position (x, y) with animation
 * frame 'frame' (tile frame * 4 + 16 of the sprites at data_027c0040), unless the
 * position is more than 16 pixels off screen. The marker meaning is a guess:
 * CampaignMap_DrawMarkers draws it for missions with the done bit. */
void CampaignMap_QueueDoneMarker(int x, int y, int frame) {
  if (x < -16 || y < -16 || x >= 256 || y >= 192) {
    return;
  }
  Sprite_QueueAt(4, x & 0x1ff, y & 0xff, &data_027c0040, 0x800, frame * 4 + 16);
}
