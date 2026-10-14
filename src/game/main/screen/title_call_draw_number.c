#include "types.h"
#include "game/oam_cells.h"

int CampaignRecords_GetNextNumber(void);
int TitleCall_GetDigitWidth(
    int digit); /* pixel width of a digit: 1 is 6, 4 is 14, others 12 */
int HwDiv32(int num, int den);
int HwMod32(int num, int den);
void Sprite_Queue(int list, int x, int y, void* data, int attr, int tile);

/* Width in pixels of the label sprite before the digits. */
#define LABEL_WIDTH 106

/* Queues the campaign record number (CampaignRecords_GetNextNumber) of the title call,
 * centred at y 76 on sprite list 2: a label sprite, then one or two digit
 * sprites with a pixel of space between them. With affineIndex 1-31 the
 * sprites use that OBJ affine matrix. */
void TitleCall_DrawNumber(int affineIndex) {
  int number;
  u16 affineBits;
  int y;
  int x;
  int width;

  number = CampaignRecords_GetNextNumber();
  width = LABEL_WIDTH;
  if (number >= 10) {
    width += TitleCall_GetDigitWidth(HwDiv32(number, 10)) + 1;
  }
  width += TitleCall_GetDigitWidth(HwMod32(number, 10));
  affineBits = 0;
  y = 0;
  if (affineIndex != 0) {
    affineBits = affineIndex << 9;
    y = 0x100;
  }
  y |= 76;
  x = (256 - width) >> 1;
  Sprite_Queue(2, (x & 0x1ff) | affineBits, y, &data_027c0148, 0x400, 0);
  x += LABEL_WIDTH;
  if (number >= 10) {
    Sprite_Queue(2, (x & 0x1ff) | affineBits, y, &data_027c0040, 0x400,
                 HwDiv32(number, 10) * 4 + 24);
    x += TitleCall_GetDigitWidth(HwDiv32(number, 10)) + 1;
  }
  Sprite_Queue(2, (x & 0x1ff) | affineBits, y, &data_027c0040, 0x400,
               HwMod32(number, 10) * 4 + 24);
}
