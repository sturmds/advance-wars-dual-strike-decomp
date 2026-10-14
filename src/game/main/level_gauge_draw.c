#include "types.h"
#include "game/level_gauge.h"
#include "game/oam_cells.h"

void Sprite_QueueAt(int list, int x, int y, void* data, int attr, int tile);
void RankSprite_QueueNumber(int x, int y, int value, int matrixIndex,
                            int frame);
void LevelGauge_QueuePoints(int x, int y, int value, int matrixIndex,
                            int frame);

extern OamCellList1 data_027c0020;
extern u8 data_027c0060[];

/* Queues gauges[index] at (x, y) unless x is off screen: the level with its
 * pop-in (levelUpFrame - 1 as the frame; levelUpFrame counts up from 1 to 255
 * once set) and the points below it, whose digits pop in from `frame`
 * (-1: at rest). Each gauge has its own affine matrices. */
void LevelGauge_Draw(LevelGauge* gauges, int x, int y, int index, int frame) {
  int spriteX;

  if (x >= 256) {
    return;
  }
  spriteX = x & 0x1ff;
  Sprite_QueueAt(7, spriteX, (y - 1) & 0xff, &data_027c0060, 0x4000, 0xc0);
  RankSprite_QueueNumber(x + 32, y - 1, gauges[index].level, index * 2 + 18,
                         gauges[index].levelUpFrame - 1);
  Sprite_QueueAt(7, spriteX, (y + 16) & 0xff, &data_027c0020, 0x4000, 0xe4);
  LevelGauge_QueuePoints(x + 24, y + 16, gauges[index].points, index * 3,
                         frame);
  if (gauges[index].levelUpFrame != 0 && gauges[index].levelUpFrame < 255) {
    gauges[index].levelUpFrame++;
  }
}
