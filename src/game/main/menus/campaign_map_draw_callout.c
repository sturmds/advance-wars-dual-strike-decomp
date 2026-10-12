#include "types.h"

int Math_Ease(int curve, int from, int to, int elapsed, int duration);
int HwDiv32(int num, int den);
void Oam_SetAffineParams(int index, s16 pa, s16 pb, s16 pc, s16 pd);
void Sprite_Queue(int list, int x, int y, void* data, int attr, int tile);

extern const s16 gSineTable[];
extern u8 data_027c00d8[];
extern u8 data_027c0128[];

/* Sine table, index 64 = cosine of 0. */
#define SIN0 (gSineTable[0])
#define COS0 (gSineTable[64])

/* Divides v by the eased scale s, or by 2 if s is 0. */
#define DIV_SCALE(v, s) HwDiv32(v, (s) != 0 ? (s) : 2)

/* Affine parameter set of the callout, and its OAM attribute 1 bits. */
#define CALLOUT_AFFINE 31
#define CALLOUT_AFFINE_INDEX (CALLOUT_AFFINE << 9)

/* OAM attribute 0 bit: affine sprite. */
#define OAM_AFFINE 0x100

/* Draws the world map callout for point (x, y) through affine matrix 31.
 * Side 0 and 1 use different cells and x offsets. Steps 0 to 16 ease its
 * height from 1/128 to full; any other step draws it at full size. It sits
 * above the point, or below it, with the cells of the other tail, when y
 * is under 32. */
void CampaignMap_DrawCallout(int x, int y, int side, int step) {
  u8* cells;
  int tile;
  int scale;

  if (side == 0) {
    cells = data_027c0128;
    x -= 32;
    tile = 32;
  } else {
    cells = data_027c00d8;
    x -= 16;
    tile = 112;
  }
  if (step < 0 || step > 16) {
    scale = 256;
  } else {
    scale = Math_Ease(0, 2, 256, step, 16);
  }
  Oam_SetAffineParams(CALLOUT_AFFINE, HwDiv32(COS0 << 4, 256),
                      DIV_SCALE(-SIN0 << 4, scale), HwDiv32(SIN0 << 4, 256),
                      DIV_SCALE(COS0 << 4, scale));
  if (y < 32) {
    if (side == 0) {
      tile += 40;
    } else {
      tile += 24;
    }
    y += 16 - (((256 - scale) << 4) >> 8);
  } else {
    y -= 32 - (((256 - scale) << 4) >> 8);
  }
  Sprite_Queue(0, (x & 0x1ff) | CALLOUT_AFFINE_INDEX, (y & 0xff) | OAM_AFFINE,
               cells, 0x1400, tile);
}
