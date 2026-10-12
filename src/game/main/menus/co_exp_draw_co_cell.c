#include "types.h"
#include "game/co_exp.h"

void DigitSprite_QueueNumberPopIn(int x, int y, int value, int matrixIndex,
                                  int frame);
void DigitSprite_QueueSmallNumber(int x, int y, int value, int matrixIndex,
                                  int frame);
void Sprite_QueueAt(int list, int x, int y, void* data, int attr, int tile);
void Sprite_QueueDefaultTile(int x, int y, int attr, int tile, int list);
void Co_QueueSmallPictureSpriteAtTile(int x, int y, int attr, int tile,
                                      int list);

extern u8 data_027c0020[];
extern u8 data_027c0060[];
extern u8 data_027c00d8[];

/* Queues one CO cell of the experience page: the CO's two pictures, the
 * level label and number, the points label and number. An empty slot
 * (level below 0) gets one placeholder sprite. After a level-up,
 * levelUpFrame counts frames (up to 255) and times the level's pop-in. */
void CoExp_DrawCoCell(CoExp* exp, int x, int y, int coSlot, int frame) {
  int pictureTile;
  int labelX;

  if (exp[coSlot].level < 0) {
    Sprite_QueueAt(6, (x + 24) & 0x1ff, y & 0xff, data_027c00d8, 0xc000, 0x344);
    return;
  }
  pictureTile = coSlot * 12;
  Co_QueueSmallPictureSpriteAtTile((x + 24) & 0x1ff, (y + 32) & 0xff,
                                   (coSlot + 5) << 12, pictureTile + 548, 7);
  Sprite_QueueDefaultTile(x & 0x1ff, y & 0xff, 0x4000, pictureTile + 476, 6);
  labelX = (x + 48) & 0x1ff;
  Sprite_QueueAt(6, labelX, (y - 1) & 0xff, data_027c0060, 0xc000, 0x30c);
  DigitSprite_QueueSmallNumber(x + 80, y - 1, exp[coSlot].level,
                               coSlot * 2 + 18, exp[coSlot].levelUpFrame - 1);
  Sprite_QueueAt(6, labelX, (y + 21) & 0xff, data_027c0020, 0xc000, 0x32c);
  DigitSprite_QueueNumberPopIn(x + 72, y + 21, exp[coSlot].points, coSlot * 3,
                               frame);
  if (exp[coSlot].levelUpFrame != 0 && exp[coSlot].levelUpFrame < 255) {
    exp[coSlot].levelUpFrame++;
  }
}
