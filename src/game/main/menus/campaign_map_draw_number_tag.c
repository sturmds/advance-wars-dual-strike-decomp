#include "types.h"
#include "game/oam_cells.h"
#include "game/math.h"

int CampaignRecords_GetNextNumber(void);
void Sprite_QueueAt(int list, int x, int y, void* data, int attr, int tile);

/* Draws a tag with the two-digit number of the next campaign record
 * (CampaignRecords_GetNextNumber) for point (x, y): a frame sprite and two digit sprites,
 * to the right of the point for side 0 and to the left otherwise. Draws
 * nothing when the frame would start off screen. */
void CampaignMap_DrawNumberTag(int x, int y, int side) {
  int number;
  int tens;
  int ones;
  int left;

  number = CampaignRecords_GetNextNumber();
  tens = HwDiv32(number, 10);
  ones = HwMod32(number, 10);
  if (side == 0) {
    left = x + 16;
    if (left < -80 || left >= 256) {
      return;
    }
    Sprite_QueueAt(3, left & 0x1ff, (y - 8) & 0xff, &data_027c0128, 0x800, 160);
    Sprite_QueueAt(2, (x + 72) & 0x1ff, y & 0xff, &data_027c0038, 0x800,
                   tens * 2 + 240);
    Sprite_QueueAt(2, (x + 80) & 0x1ff, y & 0xff, &data_027c0038, 0x800,
                   ones * 2 + 240);
  } else {
    left = x - 80;
    if (left < -80 || left >= 256) {
      return;
    }
    Sprite_QueueAt(3, left & 0x1ff, (y - 8) & 0xff, &data_027c0128, 0x800, 200);
    Sprite_QueueAt(2, (x - 24) & 0x1ff, y & 0xff, &data_027c0038, 0x800,
                   tens * 2 + 240);
    Sprite_QueueAt(2, (x - 16) & 0x1ff, y & 0xff, &data_027c0038, 0x800,
                   ones * 2 + 240);
  }
}
